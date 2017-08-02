#include <unistd.h>
#include "../src/threadpool.h"
#include "../src/debug.h"

void* func(void *arg) {
    log_info("thread %lu is working on task %d\n", (pthread_t)pthread_self(), *(int*)arg);
    sleep(1);
    return NULL;
}

#define TESTCOUNT 100
//测试代码
int main(void) {

    Threadpool pool;
    pool.init();
    int arg[TESTCOUNT];
    
    //创建十个任务
    for(int i = 0; i < TESTCOUNT; i++){
        arg[i] = i;
        pool.add_tesk(func, (void*)&arg[i]);
    }

    sleep(6);
    pool.destroy();
    return 0;
}