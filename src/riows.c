#include <riows.h>

int socket_bind (int port)
{
    int socket_fd;
    int arg = 1;
    struct sockaddr_in address;

    if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
        perror ("Error creating socket");
        exit (EXIT_FAILURE);
    }

    if (fcntl (socket_fd, F_SETFL, O_NONBLOCK) == -1) {
        perror ("Error setting socket non-blocking");
        exit (EXIT_FAILURE);
    }

    if (setsockopt (socket_fd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof (arg)) == -1) {
        perror ("Error setting SOL_REUSEADDR");
        exit (EXIT_FAILURE);
    }

    memset (&address, 0, sizeof (struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons (port);
    address.sin_addr.s_addr = inet_addr ("0.0.0.0");

    if (bind (socket_fd, &address, sizeof (struct sockaddr_in)) == -1) {
        perror ("Error on socket binding");
        exit (EXIT_FAILURE);
    }

    if (listen (socket_fd, MAX_EVENTS) == -1) {
        perror ("Error calling listen on socket");
        exit (EXIT_FAILURE);
    }

    return socket_fd;
}

int main (int argc, char **argv)
{
    int status;
    int workers = 1;
    pid_t pid;

    printf(" _ __  _   ___  \n");
    printf("| '__|| | / _ \\ \n");
    printf("| |   | || (_) |\n");
    printf("|_|   |_| \\___/ \n");
    
    rio_worker_t *worker = malloc (sizeof (rio_worker_t));
    worker->fd = socket_bind (80);
    worker->rioev = rioev_init ();
    sprintf(worker->name, "WORKER:%d", workers);

    printf ("Running %s fd:%d port:%d\n", worker->name, worker->fd, 80);
    sleep (2);

    close (worker->fd);
    exit(EXIT_SUCCESS);
}
