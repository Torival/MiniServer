#ifndef __MINISERVER_UTIL_H
#define __MINISERVER_UTIL_H

#include <errno.h>
#include "debug.h"

#define     QUEUE_LEN       50000

int openfd(int port){
    int listenfd;
    struct sockaddr_in serveraddr;
    // 接收函数返回值，以检查是否运行正确
    int ret;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    check_return(listenfd < 0, "socket init fail!\n");
    
    int optval = 1;
    ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval , sizeof(optval));  
    check_return(ret < 0, "set socket fail!\n");
    
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons(port);

    ret = bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    check_return(ret < 0, "bind error:%d, %s", errno, strerror(errno));
    
    ret = listen(listenfd, QUEUE_LEN);
    check_return(ret < 0, "listen error:%d, %s", errno, strerror(errno));

    return listenfd;
}

int set_noblock(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    
    check_return(flag == -1, "get fd error:%d, %s", errno, strerror(errno));
    flag |= O_NONBLOCK;

    int ret = fcntl(fd, F_SETFL, flag);
    check_return(ret < 0, "set fd error:%d, %s", errno, strerror(errno));

    return 0;
}

#endif