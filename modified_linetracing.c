#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringPi.h>
#include <errno.h>
#include "modified_linetracing.h"

// Function to initialize GPIO pins
void setup() {
    wiringPiSetup();
    pinMode(LEFT1, INPUT);
    pinMode(LEFT2, INPUT);
    pinMode(RIGHT1, INPUT);
    pinMode(RIGHT2, INPUT);
}

// Function to control motors via I2C
int controlMotors(int fd, unsigned char leftDir, unsigned char leftSpeed, unsigned char rightDir, unsigned char rightSpeed) {
    unsigned char data[5] = {MOTOR_REG, leftDir, leftSpeed, rightDir, rightSpeed};
    return ((write(fd, data, 5) == 5));
}
    
void turn_left(int file)
{
    printf("attempting left turn");
    int leftin = digitalRead(LEFT2);
    int rightin = digitalRead(RIGHT1);
    int flag = 0;
    while(!flag){
        flag = controlMotors(file, 0, 50, 1, 70);
    }
    delay(200);
    leftin = digitalRead(LEFT2);
    rightin = digitalRead(RIGHT1);
    while(!(leftin  == LOW || rightin == LOW))
    {
        controlMotors(file, 0, 50, 1, 70);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
    }

}

void turn_right(int file)
{
    printf("attempting right turn");
    int leftin = digitalRead(LEFT2);
    int rightin = digitalRead(RIGHT1);
    int flag = 0;
    while(!flag){
        flag = controlMotors(file, 1, 70, 0, 50);
    }
    delay(200);
    leftin = digitalRead(LEFT2);
    rightin = digitalRead(RIGHT1);
    while(!(leftin  == LOW || rightin == LOW))
    {
        controlMotors(file, 1, 70, 0, 50);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
    }


}

void go_straight(int file)
{
    int leftout = digitalRead(LEFT1);
    int rightout = digitalRead(RIGHT2);

    while((leftout  == LOW || rightout == LOW))
    {
        controlMotors(file, 1, 50, 1, 50);
        leftout = digitalRead(LEFT1);
        rightout = digitalRead(RIGHT2);
    }

}

int is_intersection(int leftout,int leftin,int rightin, int rightout)
{
    if(leftout == LOW || rightout == LOW) return 1;
    else return 0;
}
