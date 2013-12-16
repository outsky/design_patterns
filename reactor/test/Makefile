FLAGS = -g -Wall
INC = ../epoll_reactor.h ../event_set.h ../event_handler.h
SRC = ../epoll_reactor.cpp ../event_handler.cpp

all : client server

client : client_main.cpp msg.h client.h client.cpp
	g++ -o $@ ${FLAGS} client_main.cpp client.cpp -lpthread

server : server_main.cpp ${INC} ${SRC} msg.h server.h server.cpp acceptor.h acceptor.cpp logger.h logger.cpp
	g++ -o $@ ${FLAGS} ${SRC} server_main.cpp server.cpp acceptor.cpp logger.cpp -lpthread

clean :
	rm -f client server log core core.*
