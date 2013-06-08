#include <rnetwork.h>
#include <rutils.h>

int set_nonblock (int fd)
{
    return fcntl (fd, F_SETFL, O_NONBLOCK);
}

int accept_client (int fd)
{
    int client_fd;
    size_t len;
    struct sockaddr_in client;

    client_fd = accept (fd, (struct sockaddr*)&client, (socklen_t*) &len);
    log_info ("New Client fd:%d ip:%s\n", client_fd, inet_ntoa (client.sin_addr));

    return client_fd;
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

int rnetwork_loop (rio_worker_t *worker)
{
    int total;
    int new_client;
    hash *clients = hash_init (MAX_EVENTS + 100);

    worker->fd = socket_bind (80);
    worker->rioev = rioev_init ();
    sprintf(worker->name, "WORKER:%d", worker->fd);
    rioev_add (worker->rioev, worker->fd, RIOEV_IN);

    log_info ("Worker %s running\n", worker->name);

    /* riows event lopp */

    ITERATE(worker->rioev, 1000)
        EVENT_LOOP(worker->rioev)
        {
            if (worker->fd == GET_FD(ev)) {
                if (IS_RIOEV_IN(ev)) {
                    new_client = accept_client (worker->fd);
                    close (new_client);
                }
            }
        END_LOOP
        log_info (".\n");
    END_ITERATE

    log_info ("Finishing %s", worker->name);

    hash_destroy (&clients);
    rioev_destroy (&worker->rioev);
    close (worker->fd);
    free (worker);
    return 0;
}
