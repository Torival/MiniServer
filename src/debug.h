#ifndef __MINISERVER_DEBUG_H
#define __MINISERVER_DEBUG_H

#include <stdio.h>

#define log_err(M, ...)                         \
    do{                                         \
        fprintf(stderr,                         \
        "[ERROR] %s :%d : " M "\n",             \
        __FILE__, __LINE__, ##__VA_ARGS__);     \
    }while(0);                                  \


#define log_info(M, ...)                        \
    do{                                         \
        fprintf(stderr,                         \
        "[INFO] %s :%d : " M "\n",              \
        __FILE__, __LINE__, ##__VA_ARGS__);     \
    }while(0);                                  \

/*
#define check(A, M, ...) if(!(A)) { log_err(M "\n", ##__VA_ARGS__);  }

#define check_exit(A, M, ...) if(!(A)) { log_err(M "\n", ##__VA_ARGS__); exit(1);}

#define check_debug(A, M, ...) if(!(A)) { debug(M "\n", ##__VA_ARGS__); }
*/
#endif