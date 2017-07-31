#ifndef __MINISERVER_UTIL_H
#define __MINISERVER_UTIL_H

#include "debug.h"

#define     QUEUE_LEN       50000

int openfd(int port){
    int listenfd;
    struct sockaddr_in serveraddr;
    // 接收函数返回值，以检查是否运行正确
    int ret;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    check(listenfd < 0, "socket init fail!\n");
    
    
    // 调用close(socket)一般不会立即关闭socket，而经历TIME_WAIT的过程。
    int optval = 1;
    ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
        (const void *)&optval , sizeof(int));  
    check(ret < 0, "set socket fail!\n");
    
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons((unsigned short)port);

    ret = bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    check(ret < 0, "socket bind fail!\n");
    
    ret = listen(listenfd, QUEUE_LEN);
    check(ret < 0, "listen fail!\n");

    return listenfd;
}

int set_noblock(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    
    check(flag == -1, "get fd flag error!\n");
    flag |= O_NONBLOCK;

    int ret = fcntl(fd, F_SETFL, flag);
    check(ret < 0, "set fd flag error!\n");

    return 0;
}


#endif