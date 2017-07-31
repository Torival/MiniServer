#ifndef __MINISERVER_UTIL_H
#define __MINISERVER_UTIL_H

#include "debug.h"

#define     QUEUE_LEN       50000

int openfd(int port){
    int listenfd;
    struct sockaddr_in serveraddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        log_err("socket init fail!\n");
        return -1;
    }    
    
    // 调用close(socket)一般不会立即关闭socket，而经历TIME_WAIT的过程。
    int optval = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
        (const void *)&optval , sizeof(int)) < 0){
        log_err("set socket fail!\n");
        return -1;
    }

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons((unsigned short)port);

    if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
        log_err("socket bind fail!\n");
        return -1;
    }


    if (listen(listenfd, QUEUE_LEN) < 0){
        log_err("listen fail!\n");
        return -1; 
    }	       
}

int set_noblock(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    
    if(flag == -1){
        log_err("get fd flag error!\n");
        return -1;
    }

    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) < 0){
        log_err("set fd flag error!\n");
        return -1;
    }

    return 0;
}
#endif