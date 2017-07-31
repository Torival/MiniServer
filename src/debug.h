#ifndef __MINISERVER_DEBUG_H
#define __MINISERVER_DEBUG_H

#include <stdio.h>

// 输出错误信息
#define log_err(M, ...)                         \
    do{                                         \
        fprintf(stderr,                         \
        "[ERROR] %s :%d : " M "\n",             \
        __FILE__, __LINE__, ##__VA_ARGS__);     \
    }while(0);                                  \

// 输出运行相关信息
#define log_info(M, ...)                        \
    do{                                         \
        fprintf(stderr,                         \
        "[INFO] %s :%d : " M "\n",              \
        __FILE__, __LINE__, ##__VA_ARGS__);     \
    }while(0);                                  \

// 检查参数
#define check(A, M, ...)                        \
    do{                                         \
        if(A){                                  \
            log_err(M "\n", ##__VA_ARGS__);     \
            return -1;                          \
        }                                       \
    }while(0);                                  \
    
#endif