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
#elif (__APPLE__ || __FreeBSD__)
    rioev->kqfd = kqueue ();
    if (rioev->kqfd == -1) {
        free (rioev);
        return NULL;
    }
    rioev->nevents = 0;
    for (int i = 0; i < MAX_EVENTS; i++)
        rioev->changelist[i].ident = -1;
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
#elif (__APPLE__ || __FreeBSD__)
    struct kevent *_ev;
    for (int i = 0; i < MAX_EVENTS; i++) {
        _ev = &rioev->changelist[i];
        if (_ev->ident == -1) {
            _ev->ident = fd;
            EV_SET (_ev, fd, event, EV_ADD | EV_ENABLE, 0, 0, NULL);
            break;
        }
    }
    rioev->nevents++;
    return 0;
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
#elif (__APPLE__ || __FreeBSD__)
    int i = 0;
    int removed_position;
    struct kevent *_ev;
    for (i = 0; i < MAX_EVENTS; i++) {
        _ev = &rioev->changelist[i];
        if (_ev->ident == fd) {
            EV_SET (_ev, fd, _ev->filter, EV_DELETE, 0, 0, NULL);
            _ev->ident = -1;
            removed_position = i;
            break;
        }
    }
    if (i == MAX_EVENTS)
        return -1;
    /* fill empty spot */
    rioev->changelist[removed_position] = rioev->changelist[rioev->nevents - 1];
    /* empty last element, moved to the previous empty spot */
    rioev->changelist[rioev->nevents - 1].ident = -1;
    if (rioev->nevents > 0)
        rioev->nevents--;
    return 0;
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
#elif (__APPLE__ || __FreeBSD__)
    struct kevent *_ev;
    for (int i = 0; i < MAX_EVENTS; i++) {
        _ev = &rioev->changelist[i];
        if (_ev->ident == fd) {
            EV_SET (_ev, fd, event, EV_ADD | EV_ENABLE, 0, 0, NULL);
            break;
        }
    }
    return 0;
#endif
}

int rioev_poll (rioev_t *rioev, int timeout)
{
    int rc;
#ifdef __linux__
    rc = epoll_wait (rioev->epollfd, rioev->events, MAX_EVENTS, timeout);
    return rc;
#elif (__APPLE__ || __FreeBSD__)
    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000000;
    rc = kevent (rioev->kqfd, rioev->changelist, rioev->nevents,
                              rioev->eventlist, MAX_EVENTS, &ts);
    return rc;
#endif
}

void rioev_destroy (rioev_t **rioev)
{
    rioev_t *_rioev = *rioev;
#ifdef __linux__
    close (_rioev->epollfd);
#elif (__APPLE__ || __FreeBSD__)
    close (_rioev->kqfd);
#endif
    free (_rioev);
    *rioev = NULL;
}
