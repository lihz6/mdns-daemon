test: main.o
	./main.o

main.o: main.c
	gcc main.c -o main.o