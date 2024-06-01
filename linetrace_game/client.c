#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include "../server.h"

int main(){
    DGIST dgist;//DGIST 구조를 받아왔다.

    string QR_data; //QR 인식 데이터. 현재 위치.
    //두 플레이어의 데이터를 받아서 저장
    client_info player1 = dgist.player[0];
    client_info player2 = dgist.player[1];

    if(dgist.player[0].row == QR_data.at(0) && dgist.player[0].col == QR_data.at(1)){

    }

}