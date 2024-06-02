#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "client_network.h"


int main(int argc, char *argv[]){
    cAction_info cActionInf;
    ClientAction cp; 
    networking_info* nInf;
    int dpoint;
    networking_info nInfContain;
    pthread_t networkThread;

    cActionInf.cAction_point = &cp;
    cActionInf.dirty_pointer = &dpoint;
    nInf = &nInfContain;

    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);
    
    nInf->server_ip = argv[1];
    nInf->server_port_str = argv[2];
    nInf->cA_info = &cActionInf;
    
    pthread_create(&networkThread, NULL, networking, (void*)nInf);
    
    for(int i=0;i<100;i++){
        cActionInf.cAction_point->col = 1;
        cActionInf.cAction_point->row = 1;
        cActionInf.cAction_point->action = 1;
        *(cActionInf.dirty_pointer) = 1;
    }
    pthread_join(networkThread, NULL);

    return 0;
}