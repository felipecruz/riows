#include <unistd.h>
#include <rioev.h>
#include "thc.h"
#include "test_rioev.h"

void test_rioev_init (void)
{
    rioev_t *rioev = rioev_init ();

    ENSURE (NULL != rioev);
#ifdef __linux__
    ENSURE (1 <= rioev->epollfd);
#elif __APPLE__
    ENSURE (1 <= rioev->kqfd);
    ENSURE (0 == rioev->nevents);
#endif
}

void test_rioev_add_del_mod (void)
{
    int pipes[2];
    int status = pipe(pipes);
    int r = pipes[0];
    int w = pipes[1];
    rioev_t *rioev = rioev_init ();

    ENSURE (0 == rioev_add (rioev, r, RIOEV_IN));
    ENSURE (0 == rioev_del (rioev, r));

    ENSURE (0 == rioev_add (rioev, r, RIOEV_OUT));
    ENSURE (0 == rioev_mod (rioev, r, RIOEV_IN));

    ENSURE (0 == rioev_del (rioev, r));
#ifdef __linux__
    ENSURE (-1 == rioev_del (rioev, r));
#endif

    ENSURE (0 == rioev_add (rioev, r, RIOEV_IN));
    ENSURE (0 == rioev_mod (rioev, r, RIOEV_OUT));

    ENSURE (0 == rioev_del (rioev, r));
#ifdef __linux__
    ENSURE (-1 == rioev_mod (rioev, r, 0));
#endif

    ENSURE (0 == rioev_add (rioev, w, RIOEV_OUT));
    ENSURE (0 == rioev_del (rioev, w));

    ENSURE (0 == rioev_add (rioev, w, RIOEV_IN));
    ENSURE (0 == rioev_mod (rioev, w, RIOEV_OUT));

    ENSURE (0 == rioev_del (rioev, w));
#ifdef __linux__
    ENSURE (-1 == rioev_del (rioev, w));
#endif

    ENSURE (0 == rioev_add (rioev, w, RIOEV_OUT));
    ENSURE (0 == rioev_mod (rioev, w, RIOEV_IN));

    ENSURE (0 == rioev_del (rioev, w));
#ifdef __linux__
    ENSURE (-1 == rioev_mod (rioev, w, 0));
#endif
}

void test_rioev_poll (void)
{
    int pipes[2];
    int status = pipe(pipes);
    int r = pipes[0];
    int w = pipes[1];
    rioev_t *rioev = rioev_init ();

    ENSURE (0 == rioev_add (rioev, r, RIOEV_IN));
    ENSURE (0 == rioev_poll (rioev, 0));
    ENSURE (0 == rioev_add (rioev, w, RIOEV_OUT));
    ENSURE (1 == rioev_poll (rioev, 0));

    write (w, "a", 1);
    ENSURE (2 == rioev_poll (rioev, 0));
#ifdef __linux__
    ENSURE (w == rioev->events[0].data.fd);
    ENSURE (r == rioev->events[1].data.fd);
    ENSURE (RIOEV_OUT & rioev->events[0].events);
    ENSURE (RIOEV_IN & rioev->events[1].events);
#elif __APPLE__
    ENSURE (w == rioev->eventlist[0].ident);
    ENSURE (r == rioev->eventlist[1].ident);
    ENSURE (RIOEV_OUT & rioev->eventlist[0].flags);
    ENSURE (RIOEV_IN & rioev->eventlist[1].flags);
#endif

}
