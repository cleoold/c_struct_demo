_all:
	gcc -o writec structdemo_write.c darray.c -g -Wall && gcc -o readc structdemo_read.c darray.c -g -Wall
