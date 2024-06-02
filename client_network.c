#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "client_network.h"

int sock;
pthread_mutex_t lock;

void* receiveData(void* arg) {
    DGIST dgist;
    while (1) {
        int valRead = read(sock, &dgist, sizeof(DGIST));
        if (valRead < 0) {
            perror("Read error");
            exit(EXIT_FAILURE);
        }
        pthread_mutex_lock(&lock);
        // Print map and player information
        printf("========== PRINT MAP ==========\n");
        for (int i = 0; i < MAP_ROW; i++) {
            for (int j = 0; j < MAP_COL; j++) {
                Item tmpItem = dgist.map[i][j].item;
                switch (tmpItem.status) {
                    case nothing:
                        printf("- ");
                        break;
                    case item:
                        printf("%d ", tmpItem.score);
                        break;
                    case trap:
                        printf("x ");
                        break;
                }
            }
            printf("\n");
        }
        printf("========== PRINT DONE ==========\n");

        printf("========== PRINT PLAYERS ==========\n");
        for (int i = 0; i < MAX_CLIENTS; i++) {
            client_info client = dgist.players[i];
            printf("++++++++++ Player %d ++++++++++\n", i + 1);
            printf("Location: (%d, %d)\n", client.row, client.col);
            printf("Score: %d\n", client.score);
            printf("Bomb: %d\n", client.bomb);
        }
        pthread_mutex_unlock(&lock);
        printf("========== PRINT DONE ==========\n");
    }
    return NULL;
}

void* sendAction(void* arg) {
    cAction_info* cA_info = (cAction_info*)arg;
    while (1) {
        /*
        int row, col, action;
        printf("Enter your action (row col action): ");
        scanf("%d %d %d", &row, &col, &action);

        ClientAction cAction;
        cAction.row = row;
        cAction.col = col;
        cAction.action = (enum Action)action;
        
        //printf("pthread_mutex_lock - before");
        */
        pthread_mutex_lock(&lock);
        //printf("pthread_mutex_lock - after");
        //if (send(sock, &cAction, sizeof(ClientAction), 0) < 0) {
        if(*(cA_info->dirty_pointer)){
            if (send(sock, cA_info->cAction_point, sizeof(ClientAction), 0) <=0) {
                perror("Send error");
                exit(EXIT_FAILURE);
            }
        //printf("pthread_mutex_unlock - before");
        }
        printf("%d %d %d\n", cA_info->cAction_point->row, cA_info->cAction_point->col, cA_info->cAction_point->action);
        pthread_mutex_unlock(&lock);
        //printf("pthread_mutex_unlock - after");
    }
    return NULL;
}

void* networking(void* arg) {
    //if (argc != 3) {
    //    fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
    //    return 1;
    //}
    networking_info* ninfo = (networking_info*)arg;
    char* server_port_str = ninfo->server_port_str;
    char* server_ip = ninfo->server_ip;
    cAction_info* cA_info = ninfo->cA_info;

    //const char *server_ip = argv[1];
    int server_port = atoi(server_port_str);
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return NULL;
    }

    printf("Connected to server at %s:%d\n", server_ip, server_port);
    
    printf("nInfo row %d col %d action %d\n",ninfo->cA_info->cAction_point->row,ninfo->cA_info->cAction_point->col,ninfo->cA_info->cAction_point->action);

    pthread_t recvThread, sendThread;
    pthread_mutex_init(&lock, NULL);

    // Create threads for receiving and sending data
    pthread_create(&recvThread, NULL, receiveData, NULL);
    pthread_create(&sendThread, NULL, sendAction, (void*)cA_info);

    // Join threads
    pthread_join(recvThread, NULL);
    pthread_join(sendThread, NULL);

    close(sock);
    pthread_mutex_destroy(&lock);
    return NULL;
}
