main: main.o
	./main.o

main.o: main.c util.c util.h
	gcc main.c util.c util.h -o main.o

test: test.o
	./test.o

test.o: test.c host.c host.h
	gcc test.c host.c host.h -o test.o

send: send.o
	./send.o

send.o: send.c
	gcc send.c -o send.o
