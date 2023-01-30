CC = g++
CFLAGS  = -Wall

all: client server

client:  PracticalSocket.o ClientFunctions.o
	$(CC) $(CFLAGS) -o client Client.cpp PracticalSocket.o ClientFunctions.o

server:  PracticalSocket.o ServerFunctions.o
	$(CC) $(CFLAGS) -o server Server.cpp PracticalSocket.o ServerFunctions.o

PracticalSocket.o:  PracticalSocket.cpp PracticalSocket.h
	$(CC) -c PracticalSocket.cpp

ClientFunctions.o:  ClientFunctions.cpp ClientFunctions.h
	$(CC) $(CFLAGS) -c ClientFunctions.cpp

ServerFunctions.o:  ServerFunctions.cpp ServerFunctions.h
	$(CC) $(CFLAGS) -c ServerFunctions.cpp

run-cpplint: 
	cpplint --filter=-build/include_subdir Client.cpp Server.cpp ClientFunctions.h ClientFunctions.cpp ServerFunctions.h ServerFunctions.cpp

run-server: server
	./server 1550

run-client: client
	./client localhost 1550

clean:
	rm app *.o
