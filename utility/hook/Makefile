CC=gcc
CFLAGS = -Wall -shared -fPIC -ldl -lpthread
CCFLAGS = -Wall
TARGET = hookfile.so main
SRC = hookfile.c helper.c

all: ${TARGET}

hookfile.so:${SRC}
	${CC} ${SRC} -o $@ ${CFLAGS} 

main:main.c
	${CC} main.c -o $@ ${CCFLAGS}

clean:
	rm -rf ${TARGET} 
