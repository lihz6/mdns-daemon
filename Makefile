bind: bind.o
	./bind.o

bind.o: bind.c util.c util.h
	gcc bind.c util.c util.h -o bind.o


main: main.o
	./main.o

main.o: main.c
	gcc main.c -o main.o


test: test.o
	./test.o

test.o: test.c host.c host.h
	gcc test.c host.c host.h -o test.o

client: client.o
	./client.o

client.o: client.c
	gcc client.c -o client.o
