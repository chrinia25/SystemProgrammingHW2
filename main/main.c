#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringPi.h>
#include <errno.h>
#include "modified_linetracing.h"
#include "qrCode_C.h"

DGIST dgist;

int next_action = 1;
int curr_direction = 0;
int curr_node[2] = {-1,-1};
int before_node = 1;
int leftout, leftin, rightin, rightout;
int flag = 0;
inline void stable_turn_left(int file_dir){
    turn_left(file_dir);
}
inline void stable_turn_right(int file_dir){
    turn_right(file_dir);
}
inline void adjust_left(int file_dir){
    while(!(leftin == LOW || rightin == LOW)){
        flag = controlMotors(file_dir, 1, 0, 1, 50);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
    }
    flag = 0;
}
inline void adjust_right(int file_dir){
    while(!(leftin == LOW || rightin == LOW)){
        controlMotors(file_dir, 1, 50, 1, 0);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
    }
    flag = 0;
}
int find_path(int target_x, int target_y){
    if(before_node == 0){
        switch(curr_direction){
            case 0://east
                if(dgist.map[curr_node[0] + 1][curr_node[1]].item.status == 2){
                    return -2;
                }
                break;
            case 1://south
                if(dgist.map[curr_node[0]][curr_node[1] - 1].item.status == 2){
                    return -2;
                }
                break;
            case 2://west
                if(dgist.map[curr_node[0] - 1][curr_node[1]].item.status == 2){
                    return -2;
                }
                break;
            case 3://north
                if(dgist.map[curr_node[0]][curr_node[1] + 1].item.status == 2){
                    return -2;
                }
                break;
        }
    }
        
}






int main(int argc, char* argv[]){
    int file;
    const char *filename = "/dev/i2c-1";

    // Open the I2C bus
    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus\n");
        return 1;
    }

    // Specify the I2C slave address
    if (ioctl(file, I2C_SLAVE, I2C_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        close(file);
        return 1;
    }

    // Initialize GPIO pins
    setup();
    leftout = digitalRead(LEFT1);
    leftin = digitalRead(LEFT2);
    rightin = digitalRead(RIGHT1);
    rightout = digitalRead(RIGHT2);
    
    //set_communication()
    int qr;
    int temp_x;
    int temp_y;
    int player_num = -1;

    while(1){
        //QR_read
        qr = read_QR()
        if(qr != -1){
            if(player_num == -1){
                if(curr_node[0] == dgist.players[0].row && curr_node[1] == dgist.players[0].column){
                    player_num = 0;
                }   
                else if(curr_node[0] == dgist.players[1].row && curr_node[1] == dgist.players[1].column){
                    player_num = 1;
                }   
            }
            temp_x = qr / 10;
            temp_y = qr % 10;
            if(temp_x != curr_node[0] || temp_y != curr_node[1]){
                if((temp_x  == curr_node[0] + 1) && (temp_y == curr_node[1])) curr_direction = 0;
                else if((temp_x  == curr_node[0]) && (temp_y == curr_node[1] - 1)) curr_direction = 1;
                else if((temp_x  == curr_node[0] - 1) && (temp_y == curr_node[1])) curr_direction = 2;
                else if((temp_x  == curr_node[0]) && (temp_y == curr_node[1] + 1)) curr_direction = 3;
                before_node = 1;
                curr_node[0] = temp_x;
                curr_node[1] = temp_y;
                //get new direction
                if((temp_x == 1 && temp_y == 2) || (temp_x == 2 && temp_y == 1) || (temp_x == 2 && temp_y == 3) || (temp_x == 3 && temp_y == 2)){
                    send_signal(temp_x, temp_y,1);
                }
                else send_signal(temp_x, temp_y, 0);
            }
        }
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
        
        if(is_intersection){
            switch(next_action){
                case -1:
                    turn_left();
                    break();
                case 0:
                    go_straight();
                    break;
                case 1:
                    turn_right();
                    break;
            }
        }
        else if (leftout == LOW) {
            adjust_left(file);
        } else if (rightout == LOW) {
            adjust_right(file);
        } else {
            controlMotors(file,1,70,1,70);
        } 
    }
}