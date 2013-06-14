#include <riows.h>
#include <rstatic.h>

char *default_header =
    "HTTP/1.1 200 OK\r\n"
    "Server: riows 0.1\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %d\r\n\n";

char *default_response =
    "HTTP/1.1 200 OK\r\n"
    "Server: riows 0.1\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 49\r\n\n"
    "<!doctype html><body><h1>riows</h1></body></html>";

char* extension (char *value)
{
    char *p;

    while (*(value++)) {
        if (*(value) == '.')
            p = (value + 1);
        else if (*(value) == '?')
            break;
    }

    return p;
}

char* extract_query_string (char *value)
{
    char *p = NULL;
    char *query = strchr (value, '?');

    if (query == NULL)
        return NULL;

    p = malloc (sizeof (char) * (query - value));
    strcpy (p, (query + 1));

    return p;
}

const char* mime_type (char *value)
{
    if (strcmp (value, "html") == 0) {
        return "text/html";
    }
    else if (strcmp (value, "css") == 0) {
        return "text/css";
    }
    else if (strcmp (value, "json") == 0) {
        return "application/json";
    }
    else if (strcmp (value, "js") == 0) {
        return "application/javascript";
    }
    else if (strcmp (value, "png") == 0) {
        return "image/png";
    }
    else if (strcmp (value, "jpg") == 0) {
        return "image/jpg";
    }
    else if (strcmp (value, "svg") == 0) {
        return "image/svg+xml";
    }
    else if (strcmp (value, "woff") == 0) {
        return "application/font-woff";
    }
    else {
        return "plain/text";
    }

}

void handle_static (rio_worker_t *worker, rio_client_t *client)
{
    int rc;
    int file_fd;
    off_t file_len;
    char out_buffer[8192];
    char *path;
    char *file_extension;
    char *query_string;
    struct stat _stat;

    if (strcmp (client->path, "/") == 0) {
        path = malloc (strlen ("/index.html") + 2);
        path = strcpy (path, ".");
        path = strcat (path, "/index.html");
    } else {
        path = malloc (strlen (client->path) + 3);
        /* TODO: needs some sort of configuration */
        path = strcpy (path, ".");
        path = strcat (path, client->path);
    }

    log_debug ("File path: %s\n", path);
    query_string = extract_query_string (path);
    log_debug ("Query String%s\n", query_string);

    file_fd = open (path, O_RDONLY);
    if (file_fd == -1) {
        free (path);
        if (query_string)
            free (query_string);
        if (errno == ENOENT) {
            rc = write (client->fd, default_response, strlen (default_response));
            /* TODO handle rc */
            client->state = FINISHED;
            return;
        } else {
            handle_error ("Error Opening file");
        }
    }

    bzero (&_stat, sizeof (_stat));
    rc = fstat (file_fd, &_stat);
    if (rc == -1)
        handle_error ("Error fstating a file");

    log_debug ("File size:%d Offset %d\n", (int)_stat.st_size, client->current_offset);
    file_len = _stat.st_size;

    client->current_size = file_len;

    if (client->state == INIT) {
        client->current_offset = 0;
        file_extension = extension (path);

        log_debug ("File extension %s %s\n", file_extension, mime_type (file_extension));

        bzero (out_buffer, 8192);
        sprintf (out_buffer, default_header, mime_type (file_extension), file_len);
        rc = write (client->fd, out_buffer, strlen (out_buffer));
        if (rc == -1)
            handle_error ("Error on Header send");

    }

    if (query_string)
        free (query_string);
    free (path);

#ifdef __linux__
    rc = sendfile (client->fd, file_fd, (off_t*)&client->current_offset, client->current_size);
    if (rc == -1) {
        if (errno == EAGAIN) {
            handle_error ("Sendfile EAGAIN");
        } else {
            handle_error ("Sendfile error");
        }
    }

    if (client->current_offset < client->current_size) {
        client->state = SENDFILE;
        close (file_fd);
        rioev_del (worker->rioev, client->fd);
        rioev_add (worker->rioev, client->fd, RIOEV_OUT);
        return;
    }
#elif (__APPLE__ || __FreeBSD__)
#ifdef __APPLE__
    rc = sendfile (file_fd, client->fd, (off_t)client->current_offset, (off_t*)&file_len, NULL, 0);
    if (rc == -1) {
        if (errno == EAGAIN) {
            client->current_offset += file_len;
            log_debug ("Sent:%d missing: %d\n", client->current_offset, client->current_size - client->current_offset);
            client->state = SENDFILE;
            close (file_fd);
            rioev_del (worker->rioev, client->fd);
            rioev_add (worker->rioev, client->fd, RIOEV_OUT);
            return;
        } else if (errno == ENOTCONN || errno == EPIPE) {
            client->state = ERROR;
            close (file_fd);
            rioev_del (worker->rioev, client->fd);
            return;
        } else {
            handle_error ("OSX Error on Sendfile");
        }
    }
#elif __FreeBSD__
    rc = sendfile (file_fd, client->fd, (off_t)client->current_offset, file_len, NULL, (off_t*)&file_len, 0);

    /* file_len is filled only on those restrictions */
    if (((rc == -1) && (errno == EAGAIN || errno == EINTR)) || (rc == 0))
        client->current_offset += file_len;

    log_info ("Sent:%d Offset:%d Total:%d\n", file_len, client->current_offset, client->current_size);
    if (rc == -1) {
        if (errno == EAGAIN) {
            client->state = SENDFILE;
            close (file_fd);
            rioev_del (worker->rioev, client->fd);
            rioev_add (worker->rioev, client->fd, RIOEV_OUT);
            return;
        } else {
            close (file_fd);
            rioev_del (worker->rioev, client->fd);
            return;
        }
    }

    if (client->current_offset < client->current_size) {
        client->state = SENDFILE;
        close (file_fd);
        rioev_del (worker->rioev, client->fd);
        rioev_add (worker->rioev, client->fd, RIOEV_OUT);
        return;
    }
#endif
#endif

    close (file_fd);
    client->state = FINISHED;
}
