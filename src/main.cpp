#include "debug.h"
#include "util.h"

#define MAXEPOLLEVENT 1000 
int main(int argc, char* argv[]) {
    int listenfd;   
    int epfd;
    int nfds;
    int client_len;

    struct sockaddr_in clientaddr;
    struct epoll_event ev;
    struct epoll_event* ep_events = (struct ep_events *)malloc(MAXEPOLLEVENT * 
                                    sizeof(struct epoll_event));

    // sigpipe 信号屏蔽
    signal(SIGPIPE, SIG_IGN);  
    
    // 设置与要处理的事件相关的文件描述符
    ev.data.fd = listenfd;
    
    // 设置要处理的事件类型
    ev.events = EPOLLIN | EPOLLET;

    memset(&clientaddr, 0, sizeof(struct sockaddr_in));
    listenfd = open_listenfd(8080);
    set_noblock(listenfd);

    epfd = epoll_create(MAXEPOLLEVENT);
    

    while(true) {
        // 等待epoll事件的发生
        nfds = epoll_wait(epfd, ep_events, MAXEPOLLEVENT, 500);

        // 处理所发生的所有事件
        for(int i = 0; i < nfds; ++i){
            if(ep_events[i].data.fd == listenfd) {
                
            }

        }
    }
    return 0;
}
