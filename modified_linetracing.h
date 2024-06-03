// Define GPIO pins
#define LEFT1  2
#define LEFT2  3
#define RIGHT1 0	
#define RIGHT2 7

// Define I2C device address and register
#define I2C_ADDR 0x16
#define MOTOR_REG 0x01

//define function for moving
void setup();
void turn_left(int);
void turn_right(int);
void go_straight(int);
void go_back(int);
int controlMotors(int, unsigned char, unsigned char, unsigned char, unsigned char);
int is_intersection(int,int,int,int);
