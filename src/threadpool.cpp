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
    stop = false;
    
    return 0;
}

void Threadpool::destroy(){
    // 线程池已销毁过
    if(stop)
        return;
    
    stop = 1;
    cdt.broadcast();

    cdt.lock();
    
    // 销毁所有线程
    for(int i = 0; i < max_threads; ++i){
        pthread_join(threads[i], NULL);
    }

    // 销毁线程表
    free(threads);
    threads = NULL;
    
    cdt.unlock();

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
    Threadpool* pool = static_cast<Threadpool*>(arg);
    Tesk tesk;
    
    while(!pool->stop) {

        pool->cdt.lock();
        while(pool->tesk_queue.empty() && !pool->stop){
            pool->cdt.wait();
        }

        // 如果线程池停止，解锁，退出
        if(pool->stop){
            pool->cdt.unlock();
            break;
        }

        tesk = pool->tesk_queue.front();
        pool->tesk_queue.pop();
        
        pool->cdt.unlock();
        
        // 执行回调函数
        tesk.callbackfunc(tesk.args);
    }
    return NULL;
}