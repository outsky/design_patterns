FLAGS = -g -Wall
INC = ../reactor.h ../event.h ../event_handler.h
SRC = ../reactor.cpp

all : client server

client : client_main.cpp msg.h client.h client.cpp
	g++ -o $@ ${FLAGS} client_main.cpp client.cpp -lpthread

server : server_main.cpp ${INC} ${SRC} msg.h server.h server.cpp server_handler.h server_handler.cpp
	g++ -o $@ ${FLAGS} ${SRC} server_main.cpp server.cpp server_handler.cpp -lpthread

clean :
	rm -f client server log core core.*
