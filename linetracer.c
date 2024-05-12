#include <wiringPi.h> // Include WiringPi library!

const int PWM_ = 1;

int maxspeed = 1024;



int main(void)
{
    int intensity;
    wiringPiSetupGpio();
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);

    for (intensity = 0 ; intensity < maxspeed ; ++intensity)
    {
      pwmWrite(8, intensity) ;	/* provide PWM value for duty cycle */
      delay (10) ;
    }

}