FLAGS = -g -Wall

all : client server

client : cmain.cpp msg.h client.h client.cpp
	g++ -o $@ ${FLAGS} cmain.cpp client.cpp -lpthread

server : smain.cpp msg.h server.h server.cpp
	g++ -o $@ ${FLAGS} smain.cpp server.cpp -lpthread

clean :
	rm -f client server
