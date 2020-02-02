OBJ= send recv fork
all: ${OBJ}
send:
	gcc -g -o send mq_send.c -I . -Wall
recv:
	gcc -g -o recv mq_recv.c -I . -Wall
fork:
	gcc -g -o fork mq_fork.c -I . -Wall
clean:
	rm -f ${OBJ}
.PHONY: ${OBJ}
