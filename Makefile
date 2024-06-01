all:
	gcc -Wall -g server.c -o Server -lpthread
	gcc -Wall -g client_notreal.c -o Client -lpthread

