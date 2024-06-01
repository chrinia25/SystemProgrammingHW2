#include "modified_linetracing.h"


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
        if(is_intersection(leftout,leftin,rightin,rightout)) turn_left(file);
        if (leftout == LOW) {
            while(!(leftin  == LOW && rightin == LOW)){
            controlMotors(file, 0, 90, 1, 90);
            delay(1);
            leftout = digitalRead(LEFT1);
            leftin = digitalRead(LEFT2);
            rightin = digitalRead(RIGHT1);
            rightout = digitalRead(RIGHT2);
            }
        } else if (leftout == HIGH && leftin == LOW && rightin == HIGH && rightout == HIGH) {
            while(!(leftin  == LOW && rightin == LOW)){
            controlMotors(file, 1, 70, 1, 100);
            delay(1);
            leftout = digitalRead(LEFT1);
            leftin = digitalRead(LEFT2);
            rightin = digitalRead(RIGHT1);
            rightout = digitalRead(RIGHT2);
            }
        } else if (leftout == HIGH && leftin == HIGH && rightin == LOW && rightout == HIGH) {
            while(!(leftin  == LOW && rightin == LOW)){
            controlMotors(file, 1, 100, 1, 70);
            delay(1);
            leftout = digitalRead(LEFT1);
            leftin = digitalRead(LEFT2);
            rightin = digitalRead(RIGHT1);
            rightout = digitalRead(RIGHT2);
            }
        } else if (leftin  == LOW && rightin == LOW){
            controlMotors(file,1,180,1,180);
        } else controlMotors(file,0,0,0,0);
        

        // Short delay to avoid excessive I2C communication
        delay(1);
    }

    // Close the I2C device file
    close(file);

    return 0;
}
