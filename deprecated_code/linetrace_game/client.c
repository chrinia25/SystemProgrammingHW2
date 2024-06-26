#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include "../server.h"
#include "modified_linetracing.h"

#define CLOCKWISE 0
#define ANTI_CLOCKWISE 1

int my_score = 0;
int direction = 0;
int next_action;
int cycle_dir = CLOCKWISE;
client_info player_you;
client_info player_me;

void drive(int row, int col){
    int dir = [0,0,0,0];
    //동남서북으로 떨어진 거리를 계산
    if(row - player_me.row >= 0){
        dir[0] = row - player_me.row;
    }
    else{
        dir[2] = (row - player_me.row)*-1;
    }

    if(row - player_me.row >= 0){
        dir[3] = col - player_me.col;
    }
    else{
        dir[1] = (col - player_me.col)*-1;
    }

    //현재 방향이 맞으면 계속 직진
    for(int i = 0; i<dir.size(); i++){
        if(dir[i]>0 && direction == i){
            next_action = 0;
            return;
        }
    }

    //현재 방향과 목표에 따른 방향 전환
    //남
    if( direction == 1 ){
        if(dir[0]>0){
            next_action = -1;
        }
        else if(dir[2]>0){
            next_action = 1;
        }
        else{
            next_action = 1;
        }
        return;
    }
    //북
    else if(direction == 3){
        if(dir[0]>0){
            next_action = 1;
        }
        else if(dir[2]>0){
            next_action = -1;
        }
        else{
            next_action = -1;
        }
        return;
    }
    //동
    else if(direction == 0){
        if(dir[1]>0){
            next_action = 1;
        }
        else if(dir[3]>0){
            next_action = -1;
        }
        else{
            next_action = -1;
        }
        return;
    }
    //서
    else if(direction == 2){
        if(dir[1]>0){
            next_action = -1;
        }
        else if(dir[3]>0){
            next_action = 1;
        }
        else{
            next_action = 1;
        }
        return;
    }
}
void drive_outline(){
    //현재 외곽 o, clockwise일 때
    if((player_me.row == 0 ||player_me.col == 0 || player_me.row == 4 ||player_me.col == 4) && cycle_dir == CLOCKWISE){
        if(player_me.row == 0 && player_me.col < 4){
            drive(0,4);
        }
        else if(0 =< player_me.row <4 && player_me.col == 4){
            drive(4,4);
        }
        else if(player_me.row == 4 && player_me.col > 0 ){
            drive(4,0);
        }
        else if(0 < player_me.row =<4 && player_me.col == 0){
            dirve(0,0)
        }

    }
    //현재 외곽 o, counter-clockwise일 때
    else if((player_me.row == 0 ||player_me.col == 0 || player_me.row == 4 ||player_me.col == 4) && cycle_dir == ANTI_CLOCKWISE){
        if(player_me.row == 0 && 0<player_me.col =< 4){
            drive(0,0);
        }
        else if(0 =< player_me.row <4 && player_me.col == 0){
            drive(4,0);
        }
        else if(player_me.row == 4 && 0=<player_me.col < 4 ){
            drive(4,4);
        }
        else if(0 =< player_me.row <4 && player_me.col == 4){
            dirve(0,4)
        }

    }
    //아직 외곽 x
    else{
        next_action = 0;
    }
}
int main(){
    
    while(true){
        DGIST dgist;//DGIST 구조를 받아왔다.
        Node served_map[MAP_ROW][MAP_COL]; 
        int QR_data = 0; //QR 인식 데이터. 현재 위치.
        //두 플레이어의 데이터를 받아서 저장
        int QR_data_a = QR_data / 10;
        int QR_data_b = QR_data % 10;

        if(dgist.player[0].row == dgist.player[1].row && dgist.player[0].col == dgist.player[1].col){
            //두 플레이어가 같은 위치
            //bomb갯수 비교, 점수 비교(아이템이 있던 경우 점수가 바뀌므로 주의)
        }

        //두 플레이어가 다른 위치
        else if(dgist.player[0].row == QR_data_a && dgist.player[0].col == QR_data_b){
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
            break;
        }

        //계획은 어긋낫지만 폭탄이 없음
        else if(player_me.bomb == 0){
            break;
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

    while(true){
        DGIST dgist;//DGIST 구조를 받아왔다.
        Node served_map[MAP_ROW][MAP_COL]; 
        int QR_data = 0; //QR 인식 데이터. 현재 위치.
        //두 플레이어의 데이터를 받아서 저장
        int QR_data_a = QR_data / 10;
        int QR_data_b = QR_data % 10;

        if(dgist.player[0].row == dgist.player[1].row && dgist.player[0].col == dgist.player[1].col){
            //두 플레이어가 같은 위치
            //bomb갯수 비교, 점수 비교(아이템이 있던 경우 점수가 바뀌므로 주의)
        }

        //두 플레이어가 다른 위치
        else if(dgist.player[0] == QR_data_a && dgist.player[0].col == QR_data_b){
            player_you = dgist.player[1];
            player_me = dgist.player[0];
        }
        else{
            player_you = dgist.player[0];
            player_me = dgist.player[1];
        }

        served_map = dgist.map;

        //외곽 돌기
        dirve_outline()
    }
}