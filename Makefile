# 可执行文件名
EXECUTABLE := main

# 静态库目录    
LIBDIR:=     

# 静态库文件名         
LIBS := -pthread

 # 头文件目录        
INCLUDES:=./src     

# 除了当前目录外，其他的源代码文件目录     
SRCDIR:=./src   ./test          

CC:=g++
CFLAGS := -g -Wall -O3
CPPFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD

SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(SRCDIR)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
#MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.cpp,$(MISSING_DEPS)))

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

threadpooltest.o:./test/threadpooltest.cpp ./src/threadpool.h
	$(CC) -c ./test/threadpooltest.cpp

server.o:./src/server.cpp ./src/util.h
	$(CC) -c ./src/server.cpp

server.out: server.o
	$(CC) -o server.out server.o

threadpooltest.out: threadpooltest.o threadpool.o condition.o
	$(CC) -o threadpooltest.out threadpooltest.o threadpool.o condition.o $(LIBS)