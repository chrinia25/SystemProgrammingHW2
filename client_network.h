#ifndef SERVER_HEADER
#define SERVER_HEADER
#include "server.h"
#endif

void* receiveData(void* arg);
void* sendAction(void* arg);
int networking(char* server_ip, char* server_port_str,cAction_info* cA_info);

typedef struct{
    ClientAction* cAction_point;
    int* dirty_pointer; 
} cAction_info;
