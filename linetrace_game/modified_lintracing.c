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
void controlMotors(int fd, unsigned char leftDir, unsigned char leftSpeed, unsigned char rightDir, unsigned char rightSpeed) {
    unsigned char data[5] = {MOTOR_REG, leftDir, leftSpeed, rightDir, rightSpeed};
    if (write(fd, data, 5) != 5) {
        perror("Failed to write to the i2c bus\n");
    }
}

void turn_left(int file)
{
    int leftout = digitalRead(LEFT1);
    int leftin = digitalRead(LEFT2);
    int rightin = digitalRead(RIGHT1);
    int rightout = digitalRead(RIGHT2);

    while(!(leftin  == LOW && rightin == LOW))
    {
        controlMotors(file, 0, 90, 1, 90);
        delay(1);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
    }

}

void turn_right(int file)
{
    int leftout = digitalRead(LEFT1);
    int leftin = digitalRead(LEFT2);
    int rightin = digitalRead(RIGHT1);
    int rightout = digitalRead(RIGHT2);
    while(!(leftin  == LOW && rightin == LOW))
    {
        controlMotors(file, 1, 90, 0, 90);
        delay(1);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
    }


}

void go_straight(int file)
{
    int leftout = digitalRead(LEFT1);
    int leftin = digitalRead(LEFT2);
    int rightin = digitalRead(RIGHT1);
    int rightout = digitalRead(RIGHT2);

    while((leftout  == LOW || rightout == LOW))
    {
        controlMotors(file, 1, 180, 1, 180);
        delay(1);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
    }

}

int is_intersection(int leftout,int leftin,int rightin, int rightout)
{
    if((leftout==HIGH)&&(leftin==LOW)&&(rightin==LOW)&&(rightout==HIGH))//not intersection
    {
        return 0;
    }
    else if((leftout==LOW)&&(leftin==LOW)&&(rightin==LOW)&&(rightout==LOW))//not intersection;
    {
        return 0;
    }

    else if((leftout==LOW)&&(leftin==LOW)&&(rightin==LOW)&&(rightout==LOW))//is + or T corner
    {
        return 1;
    }

    else if((leftout==HIGH)&&(leftin==LOW)&&(rightin==LOW)&&(rightout==LOW))//is ㅏ or flipped ㄱ corner
    {
        return 1;
    }
    else if((leftout==LOW)&&(leftin==LOW)&&(rightin==LOW)&&(rightout==HIGH))//is ㅓ or ㄱ corner
    {
        return 1;
    }
}