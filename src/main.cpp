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
    
    ep_events = (struct ep_events *)malloc(MAXEPOLLEVENT * 
                                    sizeof(struct epoll_event));

    // sigpipe 信号屏蔽
    signal(SIGPIPE, SIG_IGN);  
    
    // 生成用于处理accept的epoll专用的文件描述符
    epfd = epoll_create(MAXEPOLLEVENT);
    // 设置与要处理的事件相关的文件描述符
    ev.data.fd = listenfd;
    // 设置要处理的事件类型
    ev.events = EPOLLIN | EPOLLET;
    // 注册epoll事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    memset(&clientaddr, 0, sizeof(struct sockaddr_in));
    listenfd = open_listenfd(8080);
    set_noblock(listenfd);

    while(true) {
        // 等待epoll事件的发生
        nfds = epoll_wait(epfd, ep_events, MAXEPOLLEVENT, 500);

        // 处理所发生的所有事件
        for(int i = 0; i < nfds; ++i){

            // 文件描述符发生错误，不可读
            if((ep_events[i].events & EPOLLERR) ||
               (ep_events[i].events & EPOLLHUP) ||
               (!(ep_events[i].events & EPOLLIN))) {

                log_err("epoll error: fd -> %d\n", ep_events.data.fd);
                close(ep_events.data.fd);
                continue;
            }    
            
            if(ep_events[i].data.fd == listenfd) {
                // 有新的连接
                int connfd, client_len;;

                connfd = accept(listenfd, (sockaddr *)&clientaddr, client_len);
                check_exit(connfd < 0, "accept error!\n");

                set_noblock(connfd);
                check(rc == 0, "make_socket_non_blocking");
                
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
                log_info("read data:%dbytes from fd:%d\n", count, sockfd);

                ev.data.fd = connfd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            } else if(ep_events[i].events & EPOLLOUT) {
                // 有数据发送
                int sockfd;

                sockfd = ep_events[i].data.fd;
                write(sockfd, line, 1024);

                ev.data.fd = sockfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }  
    }   

    free(events);
    close(listenfd);

    return 0;
}
