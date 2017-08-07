#ifndef __MINISERVER_EPOLL_EVENT_H
#define __MINISERVER_EPOLL_EVENT_H

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

#include "threadpool.h"
#include "debug.h"
#include "http.h"

#define 	MAXEPOLLEVENT 	1000

#define     QUEUE_LEN       50000

int start_server();

int openfd(int port);

int set_noblock(int fd);

void* epoll_callback(void* arg);

#endif