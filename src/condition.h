#ifndef __MINISERVER_CONDITION_H
#define __MINISERVER_CONDITION_H

#include <pthread.h>

// 封装状态
class Condition {
    public:
        Condition();
        ~Condition();
        
        // 初始化, 销毁
        int init();
        int destroy();

        // 加锁, 解锁
        int lock();
        int unlock();
    
        // 等待
        int wait();
        
        //唤醒一个, 唤醒所有
        int signal();
        int broadcast();

    private:
        pthread_mutex_t mutex;
        pthread_cond_t  cond;
};

#endif