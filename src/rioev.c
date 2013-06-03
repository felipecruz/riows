#include <rioev.h>

rioev_t* rioev_init (void)
{
    rioev_t *rioev = malloc (sizeof (rioev_t));


#ifdef __linux__
    rioev->epollfd = epoll_create (MAX_EVENTS);
    if (rioev->epollfd == -1) {
        free (rioev);
        return NULL;
    }
#elif __APPLE__
    rioev->kqfd = kqueue ();
    if (rioev->kqfd == -1) {
        free (rioev);
        return NULL;
    }
#endif


    return rioev;
}

int rioev_add (rioev_t *rioev, int fd, int event)
{
#ifdef __linux__
    int rc;
    struct epoll_event _ev;
    memset (&_ev, 0, sizeof (struct epoll_event));
    _ev.data.fd = fd;
    _ev.events = event;
    rc = epoll_ctl (rioev->epollfd, EPOLL_CTL_ADD, fd, &_ev);
    return rc;
#elif __APPLE__

#endif
}

int rioev_del (rioev_t *rioev, int fd)
{
#ifdef __linux__
    int rc;
    struct epoll_event _ev;
    memset (&_ev, 0, sizeof (struct epoll_event));
    rc = epoll_ctl (rioev->epollfd, EPOLL_CTL_DEL, fd, &_ev);
    return rc;
#elif __APPLE__

#endif

}

int rioev_mod (rioev_t *rioev, int fd, int event)
{
#ifdef __linux__
    int rc;
    struct epoll_event _ev;
    memset (&_ev, 0, sizeof (struct epoll_event));
    _ev.data.fd = fd;
    _ev.events = event;
    rc = epoll_ctl (rioev->epollfd, EPOLL_CTL_MOD, fd, &_ev);
    return rc;
#elif __APPLE__

#endif
}

int rioev_poll (rioev_t *rioev, int timeout)
{
#ifdef __linux__
    int rc;
    rc = epoll_wait (rioev->epollfd, rioev->events, MAX_EVENTS, timeout);
    return rc;
#elif __APPLE__

#endif

}
