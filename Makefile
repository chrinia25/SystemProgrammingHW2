all:
	gcc -Wall -g server.c -o Server -lpthread
	gcc -Wall -g client_notreal.c -o Client -lpthread
	gcc -Wall -g client_network.c network_header_test.c -o ClientNetworkTest -lpthread

all: net_header_test

net_header_test: network_header_test.o client_network.o
	gcc -o net_header_test client_network.o network_header_test.o

client_network.o: client_network.c client_network.h
	gcc -o client_network.o client_network.c

network_header_test.o: network_header_test.c client_network.h
	gcc -o network_header_test.o network_header_test.c



clean:
	rm -f net_header_test *.o