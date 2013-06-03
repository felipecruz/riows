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
    ENSURE (-1 == riov_mod (rioev, r, 0));
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

#ifdef __APPLE__

    /* We need more testing since rioev introduces some complexity
     * managing it's own struct kevent array.
     * In this array elements with 'ident' equal -1 are available spots.
     *
     * rioev_add - looks for the first spot
     * rioev_mod - looks for the same element to modifie it
     * rioev_del - looks for the tsame element to disable and 'free'.
     */

    int pipes1[2];
    int pipes2[2];
    int pipes3[2];
    int status1 = pipe(pipes1);
    int status2 = pipe(pipes2);
    int status3 = pipe(pipes3);
    int r1 = pipes1[0];
    int w1 = pipes1[1];
    int r2 = pipes2[0];
    int w2 = pipes2[1];
    int r3 = pipes3[0];
    int w3 = pipes3[1];

    ENSURE (0 == rioev_add (rioev, w1, RIOEV_IN));
    ENSURE (0 == rioev_add (rioev, w2, RIOEV_IN));
    ENSURE (0 == rioev_add (rioev, w3, RIOEV_IN));
    ENSURE (w1 == rioev->changelist[0].ident);
    ENSURE (w2 == rioev->changelist[1].ident);
    ENSURE (w3 == rioev->changelist[2].ident);

    ENSURE (0 == rioev_del (rioev, w2));
    ENSURE (-1 == rioev->changelist[1].ident);
    ENSURE (0 == rioev_del (rioev, w1));
    ENSURE (-1 == rioev->changelist[0].ident);

    ENSURE (0 == rioev_add (rioev, w2, RIOEV_IN));
    ENSURE (w2 == rioev->changelist[0].ident);
    ENSURE (0 == rioev_add (rioev, w1, RIOEV_IN));
    ENSURE (w1 == rioev->changelist[1].ident);
    ENSURE (0 == rioev_add (rioev, w3, RIOEV_IN));

    /* TODO verify returned events if we add same event twice */
    ENSURE (w3 == rioev->changelist[2].ident);
    ENSURE (w3 == rioev->changelist[3].ident);
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
    ITERATE(rioev)
        ENSURE (0 <= ev->ident);
        if (r == ev->ident)
            ENSURE (ev->flags & RIOEV_IN);
        if (w == ev->ident)
            ENSURE (ev->flags & RIOEV_OUT);
    }
#endif
}
