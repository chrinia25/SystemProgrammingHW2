#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringPi.h>
#include <errno.h>
#include "modified_linetracing.h"


int leftout, leftin, rightin, rightout;
int flag = 0;

inline void stable_turn_left(int file_dir){
    turn_left(file_dir);
}
inline void stable_turn_right(int file_dir){
    turn_right(file_dir);
}

inline void adjust_left(int file_dir){
    while(leftin == LOW && rightin == LOW){
        flag = controlMotors(file_dir, 0, 0, 1, 30);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
    }
    flag = 0;
}
inline void adjust_right(int file_dir){
    while(leftin == LOW && rightin == LOW){
        controlMotors(file_dir, 1, 30, 0, 0);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
    }
    flag = 0;
}
int main() {
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
    int leftout = digitalRead(LEFT1);
    int leftin = digitalRead(LEFT2);
    int rightin = digitalRead(RIGHT1);
    int rightout = digitalRead(RIGHT2);
    while (1) {
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
        printf("%d, %d, %d, %d\n",leftout,leftin,rightin,rightout);
        // Logic to control the motors based on sensor readings
        if(is_intersection(leftout,leftin,rightin,rightout)) stable_turn_right(file);
        else if (leftout == HIGH && leftin == LOW && rightin == HIGH && rightout == HIGH) {
            adjust_left(file);
        } else if (leftout == HIGH && leftin == HIGH && rightin == LOW && rightout == HIGH) {
            adjust_right(file);
        } else if (leftin  == LOW && rightin == LOW){
            controlMotors(file,1,70,1,70);
        } 
        else controlMotors(file,0,0,0,0);
        delay(20);
    }

    // Close the I2C device file
    close(file);

    return 0;
}
