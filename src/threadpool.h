#ifndef __MINISERVER_THREADPOOL_H
#define __MINISERVER_THREADPOOL_H

#include <queue>
#include "condition.h"

using std::queue;

#define THREADNUM 8

// 任务队列
struct Tesk {
    
    void* (*callbackfunc)(void* args);  // 回调函数
    void* args;                         // 回调函数参数

    Tesk(): callbackfunc(NULL), args(NULL){ }

    Tesk(void* (*callbackfunc)(void*), void* args){
        this->callbackfunc = callbackfunc;
        this->args = args;
    }
};

// 线程池
class Threadpool {
    public:
        Threadpool();
        ~Threadpool();

        int  init(int thread_num = THREADNUM);
        void destroy();
        bool add_tesk(void *(*run)(void *arg), void *arg);

    private:
        static void *thread_routine(void *arg);

        // 状态
        Condition cdt;
        
        // 任务队列
        queue<Tesk> tesk_queue;

        // 线程表
        pthread_t*  threads;
        
        // 最大线程数
        int max_threads;    
        
        // 空闲线程数
        int free_threads;
        
        // 当前拥有的线程数
        int count_thread;
        
        // 退出标记
        volatile bool stop;

};

#endif