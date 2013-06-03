#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifndef _RIOEV_H_
#define _RIOEV_H_

#define MAX_EVENTS 1024

#ifdef __linux__
#include <fcntl.h>
#include <sys/epoll.h>

#define RIOEV_IN EPOLLIN
#define RIOEV_OUT EPOLLOUT
#define RIOEV_ERR EPOLLER

#elif __APPLE__
#endif

typedef struct rioev_s rioev_t;

struct rioev_s {
    uint8_t event;

#ifdef __linux__
    int epollfd;
    struct epoll_event events[MAX_EVENTS];
#elif __APPLE__
#endif
};

rioev_t* rioev_init (void);
int rioev_add (rioev_t *rioev, int fd, int event);
int rioev_del (rioev_t *rioev, int fd);
int rioev_mod (rioev_t *rioev, int fd, int event);
int rioev_poll (rioev_t *rioev, int timeout);
#endif
