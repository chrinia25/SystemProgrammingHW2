all:
	gcc -Wall -g server.c -o Server -lpthread
	gcc -Wall -g client_notreal.c -o Client -lpthread
	gcc -Wall -g client_network.c network_header_test.c -o ClientNetworkTest -lpthread

