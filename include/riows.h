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
#include "hash.h"

#ifndef _RIOWS_H_
#define _RIOWS_H_

enum riows_client_states {
    INIT,
    SENDFILE,
    ERROR,
    FINISHED
};

typedef struct {
    pid_t pid;
    int fd;
    char name[10];
    rioev_t *rioev;
    hash *clients;
} rio_worker_t;

typedef struct {
    int fd;
    char *path;
    int method;
    enum riows_client_states state;
    off_t current_offset;
    off_t current_size;
} rio_client_t;

int main (int argc, char **argv);
#endif
