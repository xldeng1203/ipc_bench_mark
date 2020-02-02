OBJ = write read
all: ${OBJ}
read:
	gcc -g -o read fifo_read.c -I . -Wall
write:
	gcc -g -o write fifo_write.c -I . -Wall
clean:
	rm -f ${OBJ}
.PHONY: ${OBJ}
