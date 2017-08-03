#include <sys/types.h>
#include <sys/socket.h>
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

#include "debug.h"
#include "util.h"

#define MAXEPOLLEVENT 1000 

int main(int argc, char* argv[]) {
    int listenfd;   
    int epfd;
    int nfds;

    struct sockaddr_in clientaddr;
    struct epoll_event ev;
    struct epoll_event* ep_events;
    
    ep_events = (struct epoll_event *)malloc(MAXEPOLLEVENT * 
                                    sizeof(struct epoll_event));

    // sigpipe 信号屏蔽
    signal(SIGPIPE, SIG_IGN);  
    
    memset(&clientaddr, 0, sizeof(struct sockaddr_in));
    listenfd = openfd(8081);
    set_noblock(listenfd);

    // 生成用于处理accept的epoll专用的文件描述符
    epfd = epoll_create(MAXEPOLLEVENT);
    // 设置与要处理的事件相关的文件描述符
    ev.data.fd = listenfd;
    // 设置要处理的事件类型
    ev.events = EPOLLIN | EPOLLET;
    // 注册epoll事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    int index = 0;
    while(true) {
        index++;
        // 等待epoll事件的发生
        nfds = epoll_wait(epfd, ep_events, MAXEPOLLEVENT, 500);

        // 处理所发生的所有事件
        for(int i = 0; i < nfds; ++i){
            log_info("count--> %d", index);
            // 文件描述符发生错误，不可读
            if((ep_events[i].events & EPOLLERR) ||
               (ep_events[i].events & EPOLLHUP) ||
               (!(ep_events[i].events & EPOLLIN))) {

                log_err("epoll error: fd -> %d\n", ep_events[i].data.fd);
                close(ep_events[i].data.fd);
                continue;
            }    
            
            if(ep_events[i].data.fd == listenfd) {
                // 有新的连接
                int connfd ;
                socklen_t client_len;
                connfd = accept(listenfd, (sockaddr *)&clientaddr, &client_len);
                check_return(connfd < 0, "accept error!");

                int ret = set_noblock(connfd);
                check(ret != 0, "make socket no block error");
                
                log_info("new connection fd:%d", connfd);
                
                ev.data.fd = connfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            } else if(ep_events[i].events & EPOLLIN){
                // 接收到数据
                int sockfd;
                char line[1024] = {0};
                int size = 0;
                int count = 0;

                sockfd = ep_events[i].data.fd;
                while((size = read(sockfd, line, 1024)) > 0){
                    count += size;
                } 
                log_info("read data:%dbytes from fd:%d", count, sockfd);
                log_info("data:%s", line);

                ev.data.fd = sockfd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
            } else if(ep_events[i].events & EPOLLOUT) {
                // 有数据发送
                int sockfd;
                char line[1024] = {"hello world!\n"};

                sockfd = ep_events[i].data.fd;
                write(sockfd, line, 1024);

                ev.data.fd = sockfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }  
    }   

    free(ep_events);
    close(listenfd);

    return 0;
}
