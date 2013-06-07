#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <rioev.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifndef _RIOWS_H_
#define _RIOWS_H_

typedef struct {
    pid_t pid;
    int fd;
    char name[10];
    rioev_t *rioev;
} rio_worker_t;

int main (int argc, char **argv);
#endif
