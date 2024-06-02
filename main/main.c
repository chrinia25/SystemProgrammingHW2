#include "linetracer.h"

//map은 DGIST안의 map에 있고 DGIST 안에는 플레이어 데이터도 잇는데?? 이거 dgist로 변수 이름 바꾸면 안돼ㅠㅠ?
DGIST game_field;

int next_action = 1;
int curr_direction = 0;
int curr_node[2] = {0,0};
int before_node = 1;


int find_path(int target_x, int target_y){
    if(before_node == 0){
        switch(curr_direction){
            case 0://east
                if(game_field.map[curr_node[0] + 1][curr_node[1]].item.status == 2){
                    return -2;
                }
                break;
            case 1://south
                if(game_field.map[curr_node[0]][curr_node[1] - 1].item.status == 2){
                    return -2;
                }
                break;
            case 2://west
                if(game_field.map[curr_node[0] - 1][curr_node[1]].item.status == 2){
                    return -2;
                }
                break;
            case 3://north
                if(game_field.map[curr_node[0]][curr_node[1] + 1].item.status == 2){
                    return -2;
                }
                break;
        }
        return next_action;
    }

}








int main(int argc, char* argv[]){
    //set_communication()
    int qr;
    int temp_x;
    int temp_y;
    int receive = 0; //read()로 데이터를 읽어서 전역 변수 dgist가 새로 업데이트 되면 1로 바뀜
    int qr_update = 0; //우리가 QR을 읽어서 데이터를 받았는지 혹은 그 외의 이유로 데이터를 받은건지 구분하려고
    int player_num = 0; //처음 시작위치를 통해 우리가 player1인지 player2인지 아니까 그거 저장할 용도

    int qr_data; // 여기에 QR데이터 받음

    //첫 시작 위치로 
    if(qr_update == 1){
        if(qr_data/10 == 0){
            player_num = 1;
        }
        else{
            player_num = 2;
        }
    }

    while(1){
        //qr 인식 후 server에서 새로운 정보 전달
        if(receive == 1 && qr_update == 1){

        }

        //QR_read
        if(qr = read_qr != -1){
            temp_x = qr / 10;
            temp_y = qr % 10;
            if(temp_x != curr_node[0] || temp_y != curr_node[1]){
                before_node = 1;
                curr_node[0] = temp_x;
                curr_node[1] = temp_y;
                //get new direction
                if((temp_x == 1 && temp_y == 2) || (temp_x == 2 && temp_y == 1) || (temp_x == 2 && temp_y == 3) || (temp_x == 3 && temp_y == 2)){
                    //send_signal(temp_x, temp_y,1);
                }
                // else send_signal(temp_x, temp_y, 0);
            }
        }

    }
    

}