#include <cstdlib>
#include "threadpool.h"
#include "debug.h"

Threadpool::Threadpool(){

}
Threadpool::~Threadpool(){

}

int Threadpool::init(int thread_num){
    int ret = cdt.init();
    check_return(ret != 0, "Condition init fail\n");

    // 初始化线程表
    threads = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);
    
    // 初始化线程
    for(int i = 0; i < thread_num; ++i){
        pthread_create(&threads[i], NULL, thread_routine, this);
    }

    max_threads = thread_num;
    free_threads = 0;
    count_thread = 0;
    stop = false;
    
    return 0;
}

void Threadpool::destroy(){
    // 线程池已销毁过
    if(stop)
        return;

    stop = 1;
    cdt.broadcast();

    // 销毁所有线程
    for(int i = 0; i < count_thread; ++i){
        pthread_join(threads[i], NULL);
    }

    // 销毁线程表
    free(threads);
    threads = NULL;
    
    // 释放状态
    cdt.destroy();
}

bool Threadpool::add_tesk(void *(*run)(void *arg), void *arg){
    // 包装为一个任务
    Tesk tesk(run, arg);

    // 添加到任务队列
    cdt.lock();
    tesk_queue.push(tesk);
    cdt.unlock();
    cdt.signal();
}

void* Threadpool::thread_routine(void *arg){
    Tesk tesk;
    
    while(!stop) {

        cdt.lock();
        while(tesk_queue.empty() && !stop){
            cdt.wait();
        }

        // 如果线程池停止，解锁，退出
        if(stop){
            cdt.unlock();
            break;
        }

        // if(tesk_queue.empty())
        //     continue;

        tesk = tesk_queue.front();
        tesk_queue.pop();
        
        cdt.unlock();
        
        // 执行回调函数
        tesk.callbackfunc(tesk.args);
    }
}