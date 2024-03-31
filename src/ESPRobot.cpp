#include "RobotCar.h"

RobotCar::RobotCar(PinName rwPin, PinName lwPin, PinName in1Pin, PinName in2Pin, PinName in3Pin, PinName in4Pin)
    : rightWheel(rwPin), leftWheel(lwPin), in1(in1Pin), in2(in2Pin), in3(in3Pin), in4(in4Pin), timePeriod(0.00009) {}

void RobotCar::forward(float rightWheelSpeed, float leftWheelSpeed) {
    in1 = 0;
    in3 = 0;
    in2 = 1;
    in4 = 1;
    rightWheel.period(timePeriod);
    rightWheel.write(rightWheelSpeed);
    leftWheel.period(timePeriod);
    leftWheel.write(leftWheelSpeed);
}

void RobotCar::takeTurn(float rightWheelSpeed, float leftWheelSpeed) {
    in1 = 0;
    in3 = 0;
    in2 = 1;
    in4 = 1;
    rightWheel.period(timePeriod);
    rightWheel.write(rightWheelSpeed);
    leftWheel.period(timePeriod);
    leftWheel.write(leftWheelSpeed);
}

void RobotCar::onSpotTurn(float rightWheelSpeed, float leftWheelSpeed) {
    in1 = 1;
    in3 = 0;
    in2 = 1;
    in4 = 1;
    rightWheel.period(timePeriod);
    rightWheel.write(rightWheelSpeed);
    leftWheel.period(timePeriod);
    leftWheel.write(leftWheelSpeed);
}
