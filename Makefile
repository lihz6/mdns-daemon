main: main.o
	./main.o

main.o: main.c util.c util.h host.c host.h ipad.c ipad.h
	gcc -std=gnu11 main.c util.c util.h host.c host.h ipad.c ipad.h -o main.o

test: test.o
	./test.o

test.o: test.c host.c host.h
	gcc -std=gnu11 test.c host.c host.h -o test.o

send: send.o
	./send.o

send.o: send.c
	gcc -std=gnu11 send.c -o send.o
