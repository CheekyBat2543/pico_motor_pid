#ifndef ROVER_MAIN_HPP
#define ROVER_MAIN_HPP

#include "motor.h"
// #include "pico/stdlib.h"

struct MotorSet {
    MotorSet(const uint motorFl, const uint motorFr, const uint motorBl, const uint motorBr) : 
        frontLeft(motorFl), frontRight(motorFr), backLeft(motorBl), backRight(motorBr) {}

    MotorSet(const MotorSet& motors) : 
        frontLeft(motors.frontLeft), frontRight(motors.frontRight), backLeft(motors.backLeft), backRight(motors.backRight) {}
        
    Motor frontLeft{0};
    Motor frontRight{0};
    Motor backLeft{0};
    Motor backRight{0};
};
struct ServoSet {
    ServoSet(const uint motorFl, const uint motorFr, const uint motorBl, const uint motorBr) : 
        frontLeft(motorFl), frontRight(motorFr), backLeft(motorBl), backRight(motorBr) {}

    ServoSet(const ServoSet& servos) : 
        frontLeft(servos.frontLeft), frontRight(servos.frontRight), backLeft(servos.backLeft), backRight(servos.backRight) {}

    Servo frontLeft{0};
    Servo frontRight{0};
    Servo backLeft{0};
    Servo backRight{0};
};

#endif