#include <riows.h>
#include <rnetwork.h>
#include <rutils.h>

static volatile int interrupted = 0;

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
    if (rc == -1)
        handle_error ("Error installing termination signal actions");
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


    if ((pid = fork ()) == -1)
        handle_error ("Error Forking");

    rio_worker_t *worker = malloc (sizeof (rio_worker_t));

    if (pid == 0) {
        rnetwork_loop (worker);
        exit (EXIT_SUCCESS);
    }

    working_dir_name = getcwd (NULL, 0);
    setup_termination_signals ();

    log_info ("Working dir: %s\n", working_dir_name);

    while (1) {
        sleep (0.1);
        if (interrupted) {
            log_info ("Program interrupted...\nExiting\n");
            break;
        }
    }

    free (working_dir_name);
    exit(EXIT_SUCCESS);
}
