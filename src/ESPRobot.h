#ifndef ROBOTCAR_H
#define ROBOTCAR_H

#include "mbed.h"

class RobotCar {
public:
    RobotCar(PinName rwPin, PinName lwPin, PinName in1Pin, PinName in2Pin, PinName in3Pin, PinName in4Pin);

    void forward(float rightWheelSpeed, float leftWheelSpeed);
    void takeTurn(float rightWheelSpeed, float leftWheelSpeed);
    void onSpotTurn(float rightWheelSpeed, float leftWheelSpeed);

private:
    PwmOut rightWheel;
    PwmOut leftWheel;
    DigitalOut in1;
    DigitalOut in2;
    DigitalOut in3;
    DigitalOut in4;
    float timePeriod;
};

#endif
