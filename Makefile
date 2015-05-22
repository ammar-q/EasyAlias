CC = gcc
CFLAGS = -g -Wall

all: init

init: deliver
	easyalias -i

deliver: easyalias
	sudo mv easyalias /bin/

easyalias: easyalias.o
	${CC} ${CFLAGS} -o $@ easyalias.o  

%.o : %.c 
	${CC} ${CFLAGS}  -c $<

