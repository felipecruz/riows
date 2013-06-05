#include <riows.h>

static volatile int interrupted = 0;

int rnetwork_loop (rio_worker_t *worker)
{
    printf ("Worker %s running\n", worker->name);
    sleep (1);
    rioev_destroy (&worker->rioev);
    close (worker->fd);
    free (worker);
    return 0;
}

void interrupt (int signal)
{
    interrupted = 1;
}

void setup_termination_signals ()
{
    int rc;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = interrupt;
    sa.sa_flags = SA_RESTART;

    rc = sigaction (SIGINT, &sa, NULL);
    if (rc == -1) {
        perror ("Error installing termination signal actions");
        exit (EXIT_FAILURE);
    }
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

    if (fcntl (socket_fd, F_SETFL, O_NONBLOCK) == -1) {
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

int main (int argc, char **argv)
{
    int status;
    int workers = 1;
    char *working_dir_name;
    pid_t pid;

    printf (" _ __  _   ___  \n");
    printf ("| '__|| | / _ \\ \n");
    printf ("| |   | || (_) |\n");
    printf ("|_|   |_| \\___/ \n");


    if ((pid = fork ()) == -1) {
        perror ("Error Forking");
        exit (EXIT_FAILURE);
    }

    if (pid == 0) {
        rio_worker_t *worker = malloc (sizeof (rio_worker_t));
        worker->fd = socket_bind (80);
        worker->rioev = rioev_init ();
        sprintf(worker->name, "WORKER:%d", workers);
        rnetwork_loop (worker);
        exit (EXIT_SUCCESS);
    }

    working_dir_name = getcwd (NULL, 0);
    setup_termination_signals ();

    printf ("Working dir: %s\n", working_dir_name);

    while (1) {
        sleep (0.1);
        if (interrupted) {
            printf ("Program interrupted...\nExiting\n");
            break;
        }
    }

    free (working_dir_name);
    exit(EXIT_SUCCESS);
}
