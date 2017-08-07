#include "epoll_event.h"

int start_server() {
	int listenfd;   
    int epfd;
    int nfds;

    struct sockaddr_in clientaddr;
    struct epoll_event ev;
    struct epoll_event* ep_events;
    
    ep_events = (struct epoll_event *)malloc(MAXEPOLLEVENT * 
                                    sizeof(struct epoll_event));

	// TODO
	Threadpool pool;
	pool.init();
    
	// sigpipe 信号屏蔽
    signal(SIGPIPE, SIG_IGN);  
    
    memset(&clientaddr, 0, sizeof(struct sockaddr_in));
    listenfd = openfd(8080);
    set_noblock(listenfd);

    // 生成用于处理accept的epoll专用的文件描述符
    epfd = epoll_create(MAXEPOLLEVENT);
    // 设置与要处理的事件相关的文件描述符
    ev.data.fd = listenfd;
    // 设置要处理的事件类型
    ev.events = EPOLLIN | EPOLLET;
    // 注册epoll事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    
    while(true) {
        // 等待epoll事件的发生
        nfds = epoll_wait(epfd, ep_events, MAXEPOLLEVENT, 500);

        // 处理所发生的所有事件
        for(int i = 0; i < nfds; ++i){
            
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
                while(true) {
                    int connfd ;
                    socklen_t client_len;
                    
                    connfd = accept(listenfd, (sockaddr *)&clientaddr, &client_len);
                    if (connfd < 0) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            break;
                        } else {
                            log_err("accept fail");
                            break;
                        }
                    }

                    int ret = set_noblock(connfd);
                    check(ret != 0, "make socket no block error");
                    
                    log_info("new connection fd:%d", connfd);
                    
                    ev.data.fd = connfd;
                    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                }  
            } else if(ep_events[i].events & EPOLLIN){
                int sockfd = ep_events[i].data.fd;

                // HttpRequest rqst;
	            // int size = rqst.readData(sockfd);
                // log_info("from socket:%d read data size: %d", sockfd, size); 

                // if(size == 0){
                //     close(sockfd); 
                //     break;
                // }
                
                // if(size == -1 && errno == EAGAIN)
                //     break;

                // log_info("thread ID:%lu is working", (pthread_t)pthread_self());
                // HttpResponse rsps(sockfd, rqst.getUri());
                // rsps.response_file();
                // close(sockfd); 
                pool.add_tesk(epoll_callback, (void*)&sockfd);     
            }
        } // end of epoll wait
    }   

    free(ep_events);
    close(listenfd);
	pool.destroy();
	return -1;
}

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

void* epoll_callback(void *arg) {
    int sockfd = *(int*)arg;            
	HttpRequest rqst;
	int size = rqst.readData(sockfd);

    log_info("from socket:%d read data size: %d", sockfd, size); 

    if(size == 0){
        close(sockfd); 
        return NULL;
    }
    
    if(size == -1 && errno == EAGAIN)
        return NULL;

	log_info("thread ID:%lu is working", (pthread_t)pthread_self());
	HttpResponse rsps(sockfd, rqst.getUri());
	rsps.response_file();
	close(sockfd); 

    return NULL;
}