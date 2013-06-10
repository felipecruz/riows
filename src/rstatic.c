#include <riows.h>
#include <rstatic.h>

char *default_header =
    "HTTP/1.1 200 OK\r\n"
    "Server: riows 0.1\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %d\r\n\n";

void handle_static (rio_worker_t *worker, rio_client_t *client)
{
    int rc;
    int file_fd;
    size_t file_len;
    char out_buffer[8192];
    struct stat _stat;

    file_fd = open ("site/index.html", O_RDONLY);
    if (file_fd == -1)
        handle_error ("Error Opening file");

    bzero (&_stat, sizeof (_stat));
    rc = fstat (file_fd, &_stat);
    if (rc == -1)
        handle_error ("Error fstating a file");

    log_debug ("File size:%d\n", _stat.st_size);
    file_len = _stat.st_size;

    sprintf (out_buffer, default_header, "text/html", file_len);
    rc = write (client->fd, out_buffer, strlen (out_buffer));
    if (rc == -1)
        handle_error ("Error on Header send");

    rc = sendfile (file_fd, client->fd, 0, &file_len, NULL, 0);
    if (rc == -1)
        handle_error ("Error on Kernel Sendfile");

    close (file_fd);
    client->state = FINISHED;
}
