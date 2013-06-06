#include <rnetwork.h>

int set_nonblock (int fd)
{
    return fcntl (fd, F_SETFL, O_NONBLOCK);
}

int accept_client (int fd)
{
    int client_fd;
    struct sockaddr_in client;

    client_fd = accept (fd, NULL, 0);

    printf ("New Client %d\n", client_fd);

    return client_fd;
}

int socket_bind (int port)
{
    int socket_fd;
    int arg = 1;
    struct sockaddr_in address;

    if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
        perror ("Error creating socket");
        exit (EXIT_FAILURE);
    }

    if (set_nonblock (socket_fd) == -1) {
        perror ("Error setting socket non-blocking");
        exit (EXIT_FAILURE);
    }

    if (setsockopt (socket_fd, SOL_SOCKET, SO_REUSEADDR, &arg,
                                                         sizeof (arg)) == -1) {
        perror ("Error setting SOL_REUSEADDR");
        exit (EXIT_FAILURE);
    }

    memset (&address, 0, sizeof (struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons (port);
    address.sin_addr.s_addr = inet_addr ("0.0.0.0");

    if (bind (socket_fd, (const struct sockaddr *)&address,
                         sizeof (struct sockaddr_in)) == -1) {
        perror ("Error on socket binding");
        exit (EXIT_FAILURE);
    }

    if (listen (socket_fd, MAX_EVENTS) == -1) {
        perror ("Error calling listen on socket");
        exit (EXIT_FAILURE);
    }

    return socket_fd;
}

int rnetwork_loop (rio_worker_t *worker)
{
    int total;
    int new_client;

    worker->fd = socket_bind (80);
    worker->rioev = rioev_init ();
    sprintf(worker->name, "WORKER:%d", worker->fd);
    rioev_add (worker->rioev, worker->fd, RIOEV_IN);

    printf ("Worker %s running\n", worker->name);

    /* riows event lopp */

    ITERATE(worker->rioev, 1000)
        EVENT_LOOP(worker->rioev)
        {
            if (worker->fd == GET_FD(ev)) {
                if (IS_RIOEV_IN(ev)) {
                    new_client = accept_client (worker->fd);
                    printf ("Client %d Connected\n", new_client);
                    close (new_client);
                }
            }
        END_LOOP
        printf (".\n");
    END_ITERATE

    printf ("Finishing %s", worker->name);
    rioev_destroy (&worker->rioev);
    close (worker->fd);
    free (worker);
    return 0;
}
