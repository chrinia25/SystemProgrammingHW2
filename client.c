#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include "server.h"

int my_score = 0;

client_info player_you;
client_info player_me;

void drive(int row, int col){

}

int main(){
    DGIST dgist;//DGIST 구조를 받아왔다.
    Node served_map[MAP_ROW][MAP_COL]; 
    string QR_data = ""; //QR 인식 데이터. 현재 위치.
    //두 플레이어의 데이터를 받아서 저장

    if(dgist.player[0].row == dgist.player[1].row && dgist.player[0].col == dgist.player[1].col){
        //두 플레이어가 같은 위치
        //bomb갯수 비교, 점수 비교(아이템이 있던 경우 점수가 바뀌므로 주의)
    }

    //두 플레이어가 다른 위치
    else if(dgist.player[0].row == QR_data.at(0) && dgist.player[0].col == QR_data.at(1)){
        player_you = dgist.player[1];
        player_me = dgist.player[0];
    }
    else{
        player_you = dgist.player[0];
        player_me = dgist.player[1];
    }

    served_map = dgist.map;

    //계획대로 폭탄을 전부 놓음.
    if(served_map[3][2].score == -8 && served_map[2][3].score == -8 && served_map[1][2].score == -8 && served_map[2][1].score == -8 &&){
        //외곽 돌기
    }

    //계획은 어긋낫지만 폭탄이 없음
    else if(player_me.bomb == 0){
        //외곽 돌기
    }

    //폭탄 설치하러 가기
    else(){
        if(served_map[3][2].score != -8){
            drive(3,2);
        }
        else if(served_map[2][3].score != -8){
            drive(2,3);
        }
        else if(served_map[1][2].score != -8){
            drive(1,2);
        }
        else if(served_map[2][1].score != -8){
            drive(2,1);
        }

    }


}