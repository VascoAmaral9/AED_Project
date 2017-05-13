CC=gcc
CFLAGS=-Wall -ansi -g

autopark: Gestor.o LinkedList.o Graph.o Files.o Heap.o Parking.o Queue.o escreve_saida.o
	$(CC) $(CFLAGS) -o autopark Gestor.o LinkedList.o Graph.o Files.o Heap.o Parking.o Queue.o escreve_saida.o

escreve_saida.o: escreve_saida.c Files.h
	$(CC) -c $(CFLAGS) escreve_saida.c

Gestor.o: Gestor.c Parking.h
	$(CC) -c $(CFLAGS) Gestor.c

Parking.o: Parking.c Parking.h
	$(CC) -c $(CFLAGS) Parking.c

LinkedList.o: LinkedList.c LinkedList.h defs.h
	$(CC) -c $(CFLAGS) LinkedList.c

Heap.o: Heap.c Heap.h
	$(CC) -c $(CFLAGS) Heap.c

Queue.o: Queue.c Queue.h
	$(CC) -c $(CFLAGS) Queue.c

Graph.o: Graph.c Graph.h
	$(CC) -c $(CFLAGS) Graph.c

Files.o: Files.c Files.h
	$(CC) -c $(CFLAGS) Files.c

clean::
	rm -f *.o core a.out newWords *~



