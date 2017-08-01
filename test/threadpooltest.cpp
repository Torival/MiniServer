#include "../src/threadpool.h"
#include "../src/debug.h"

void* func(void *arg) {
    log_info("thread %d is working on task %d\n", (int)pthread_self(), *(int*)arg);
    sleep(1);
    return NULL;
}

//测试代码
int main(void) {

    Threadpool pool;
    pool.init();
    char arg = 'A';
    
    //创建十个任务
    for(int i = 0; i < 100; i++){
        pool.add_tesk(func, (void*)arg);
        arg++;
    }

    pool.destory();
    return 0;
}