CC=gcc
BASE_DIR= .
BASE_INC= ${BASE_DIR}/include

INC= -I${BASE_INC}
LIBDIR= 
LIB= 
COMPILE_DIR=compile
FLAG= ${LIBDIR} ${LIB} ${INC} -g


TARGET=memcached
SRCS=$(wildcard src/*.c)
SRCSNOTDIR=$(notdir $(SRCS))
OBJS=$(patsubst %.c, compile/%.o, $(SRCSNOTDIR)) 

.PHONY: clean 

#$(shell if [ -f $(COMPILE_DIR) ]; then echo "Existed!"; else echo "NO" ;  fi;)

$(TARGET):$(OBJS)
	${CC} -o $@ $^ ${FLAG} 

compile/%.o:src/%.c
	${CC} -o $@ -c $< ${FLAG} 

clean :
	rm -rf $(TARGET) $(OBJS) 


