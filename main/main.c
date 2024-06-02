#include "linetracer.h"


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
    int temp_y
    while(1){
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