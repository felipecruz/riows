#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#ifndef _RIOEV_H_
#define _RIOEV_H_

#define MAX_EVENTS 1024

#ifdef __linux__
#include <sys/epoll.h>

#define RIOEV_IN EPOLLIN
#define RIOEV_OUT EPOLLOUT
#define RIOEV_ERR EPOLLER

#define GET_FD(ev) (int)ev->data.fd
#define IS_RIOEV_IN(ev) (ev->events & RIOEV_IN)
#define IS_RIOEV_OUT(ev) (ev->events & RIOEV_OUT)

#define ITERATE(ctx)                                                    \
    struct epoll_event *ev;                                             \
    int total = rioev_poll (ctx, 0);                                    \
    for (int i = 0; i < total; i++) {                                   \
        ev = &ctx->events[i];                                           \

#elif __APPLE__
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#define RIOEV_IN EVFILT_READ
#define RIOEV_OUT EVFILT_WRITE
#define RIOEV_ERR 0

#define GET_FD(ev) (int)ev->ident
#define IS_RIOEV_IN(ev) (ev->flags & RIOEV_IN)
#define IS_RIOEV_OUT(ev) (ev->flags & RIOEV_OUT)

#define ITERATE(ctx)                                                   \
    struct kevent *ev;                                                 \
    int total = rioev_poll (ctx, 0);                                   \
    for (int i = 0; i < total; i++) {                                  \
        ev = &ctx->eventlist[i];
#endif

typedef struct rioev_s rioev_t;

struct rioev_s {
#ifdef __linux__
    int epollfd;
    struct epoll_event events[MAX_EVENTS];
#elif __APPLE__
    int kqfd;
    int nevents;
    struct kevent changelist[MAX_EVENTS];
    struct kevent eventlist[MAX_EVENTS];
#endif
};

rioev_t* rioev_init (void);
int rioev_add (rioev_t *rioev, int fd, int event);
int rioev_del (rioev_t *rioev, int fd);
int rioev_mod (rioev_t *rioev, int fd, int event);
int rioev_poll (rioev_t *rioev, int timeout);
#endif
