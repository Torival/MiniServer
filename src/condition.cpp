#include <errno.h>
#include <string.h>
#include "condition.h"
#include "debug.h"

Condition::Condition() {

}

Condition::~Condition() {

}

int Condition::init() {
    int ret;

    ret = pthread_mutex_init(&mutex, NULL);
    check_return(ret != 0, "mutex init fail\n");

    ret = pthread_cond_init(&cond, NULL);
    check_return(ret != 0, "cond init fail\n");

    return 0;
}

int Condition::destroy() {
    int ret;

    ret = pthread_cond_destroy(&cond);
    check_return(ret != 0, "cond destroy fail, error:%d, %s\n", ret, strerror(ret));

    ret = pthread_mutex_destroy(&mutex);
    check_return(ret != 0, "mutex destroy fail, error:%d, %s\n", ret, strerror(ret));
    
    return 0;
}

int Condition::lock(){
    return pthread_mutex_lock(&mutex);
}

int Condition::unlock(){
    return pthread_mutex_unlock(&mutex);
}

int Condition::wait(){
    return pthread_cond_wait(&cond, &mutex);
}

int Condition::signal(){
    return pthread_cond_signal(&cond);
}

int Condition::broadcast(){
    return pthread_cond_broadcast(&cond);
}