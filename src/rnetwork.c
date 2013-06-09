#include <rnetwork.h>
#include <rutils.h>

char *default_response =
    "HTTP/1.1 200 OK\r\nContent-Type: text/html;"
    "charset=utf-8\r\nContent-Length: 50\r\n\n"
    "<!doctype html><body><h1>riows</h1></body></html>";

int set_nonblock (int fd)
{
    return fcntl (fd, F_SETFL, O_NONBLOCK);
}

int accept_client (int fd, rio_client_t *rio_client)
{
    size_t len;
    struct sockaddr_in client;

    rio_client->fd = accept (fd, (struct sockaddr*)&client, (socklen_t*) &len);

    if (set_nonblock (rio_client->fd) == -1)
        handle_error ("Error setting socket non-blocking");

    log_debug ("New Client fd:%d ip:%s\n", rio_client->fd, inet_ntoa (client.sin_addr));

    return rio_client->fd;
}

int socket_bind (int port)
{
    int socket_fd;
    int arg = 1;
    struct sockaddr_in address;

    if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        handle_error ("Error creating socket");

    if (set_nonblock (socket_fd) == -1)
        handle_error ("Error setting socket non-blocking");

    if (setsockopt (socket_fd, SOL_SOCKET, SO_REUSEADDR, &arg,
                                                         sizeof (arg)) == -1)
        handle_error ("Error setting SOL_REUSEADDR");

    memset (&address, 0, sizeof (struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons (port);
    address.sin_addr.s_addr = inet_addr ("0.0.0.0");

    if (bind (socket_fd, (const struct sockaddr *)&address,
                         sizeof (struct sockaddr_in)) == -1)
        handle_error ("Error on socket binding");

    if (listen (socket_fd, MAX_EVENTS) == -1)
        handle_error ("Error calling listen on socket");

    return socket_fd;
}

void handle_request (rio_worker_t *worker, rio_client_t *client)
{
    int rc;
    char buffer[8192];

    rc = recv (client->fd, buffer, 8192, MSG_DONTWAIT);
    if (rc == -1) {
        log_err ("Error Reading");
        exit (-1);
    }
    log_debug ("Client fd:%d - Request - size:%d\n %s\n", client->fd, rc, buffer);

    rc = send (client->fd, default_response, strlen (default_response), MSG_DONTWAIT);
    if (rc == -1) {
        log_err ("Error Writing");
        exit (-1);
    }

    rioev_del (worker->rioev, client->fd);
    close (client->fd);
}

int rnetwork_loop (rio_worker_t *worker)
{
    int total;
    int new_client;
    hash *clients = hash_init (MAX_EVENTS + 100);
    hash_elem_t *el;

    worker->fd = socket_bind (80);
    worker->rioev = rioev_init ();
    sprintf(worker->name, "WORKER:%d", worker->fd);
    rioev_add (worker->rioev, worker->fd, RIOEV_IN);

    log_info ("Worker %s running\n", worker->name);

    /* riows event lopp */

    ITERATE(worker->rioev, 200)
        rio_client_t *new_client;
        log_debug ("Events: %d\n", total);
        EVENT_LOOP(worker->rioev)
        {
            log_debug ("Worker fd:%d event fd:%d\n", worker->fd, GET_FD(ev));
            if (worker->fd == GET_FD(ev)) {
                if (IS_RIOEV_IN(ev)) {
                    new_client = malloc (sizeof (rio_client_t));
                    if (accept_client (worker->fd, new_client) == -1)
                        log_err ("Error Accepting Client");
                    else {
                        hash_put (clients, new_client->fd , new_client,
                                  sizeof (new_client));
                        rioev_add (worker->rioev, new_client->fd, RIOEV_IN);
                    }
                }
            } else {
                el = hash_get (clients, GET_FD(ev));
                handle_request (worker, el->value);
            }
        END_LOOP
    END_ITERATE

    log_info ("Finishing %s", worker->name);

    hash_destroy (&clients);
    rioev_destroy (&worker->rioev);
    close (worker->fd);
    free (worker);
    return 0;
}
