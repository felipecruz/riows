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

#elif __APPLE__
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#define RIOEV_IN EVFILT_READ
#define RIOEV_OUT EVFILT_WRITE
#define RIOEV_ERR 0
#endif

typedef struct rioev_s rioev_t;

struct rioev_s {
#ifdef __linux__
    int epollfd;
    struct epoll_event events[MAX_EVENTS];
#elif __APPLE__
    int kqfd;
#endif
};

rioev_t* rioev_init (void);
int rioev_add (rioev_t *rioev, int fd, int event);
int rioev_del (rioev_t *rioev, int fd);
int rioev_mod (rioev_t *rioev, int fd, int event);
int rioev_poll (rioev_t *rioev, int timeout);
#endif
