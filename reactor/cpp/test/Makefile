FLAGS = -g -Wall

all : client server

client : client_main.cpp msg.h client.h client.cpp
	g++ -o $@ ${FLAGS} client_main.cpp client.cpp -lpthread

server : server_main.cpp msg.h server.h server.cpp
	g++ -o $@ ${FLAGS} server_main.cpp server.cpp -lpthread

clean :
	rm -f client server log
