# 可执行文件名
EXECUTABLE := server

# 静态库目录    
LIBDIR:=     

# 静态库文件名         
LIBS := -pthread

 # 头文件目录        
INCLUDES:=./src     

# 除了当前目录外，其他的源代码文件目录     
SRCDIR:=./src   ./test          

CC:=g++
CC += -std=c++11
CFLAGS := -g -Wall -O3
CPPFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD

SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(SRCDIR)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
#MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.cpp,$(MISSING_DEPS)))

server.out: server.o http.o threadpool.o condition.o http.o epoll_event.o
	$(CC) -o server.out server.o http.o threadpool.o condition.o epoll_event.o $(LIBS)

all: $(EXECUTABLE)

deps : $(DEPS)

objs : $(OBJS)

clean :
	rm -f *.o
	rm -f *.out
veryclean: clean
	@$(RM-F) $(EXECUTABLE)

rebuild: veryclean all

#ifneq ($(MISSING_DEPS),)
#$(MISSING_DEPS) :
#	@$(RM-F) $(patsubst %.d,%.o,$@)
#endif
-include $(DEPS)
$(EXECUTABLE) : $(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))


threadpool.o: ./src/threadpool.cpp ./src/threadpool.h 
	$(CC) -c ./src/threadpool.cpp

condition.o: ./src/condition.cpp ./src/condition.h 
	$(CC) -c ./src/condition.cpp

http.o: ./src/http.cpp ./src/http.h 
	$(CC) -c ./src/http.cpp

epoll_event.o:./src/epoll_event.cpp ./src/epoll_event.h ./src/threadpool.h
	$(CC) -c ./src/epoll_event.cpp

server.o:src/server.cpp src/epoll_event.h src/threadpool.h \
 src/condition.h src/debug.h src/http.h
	$(CC) -c ./src/server.cpp
	
threadpooltest.o:./test/threadpooltest.cpp ./src/threadpool.h
	$(CC) -c ./test/threadpooltest.cpp

threadpooltest.out: threadpooltest.o threadpool.o condition.o
	$(CC) -o threadpooltest.out threadpooltest.o threadpool.o condition.o $(LIBS)

testrequest.o:./test/testrequest.cpp ./src/testrequest.h
	$(CC) -c ./test/testrequest.cpp

testrequest.out: testrequest.o http.o
	$(CC) -o testrequest.out testrequest.o http.o