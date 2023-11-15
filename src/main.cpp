/**
 * @file main.cpp
 * @author Alper Tunga Güven (alpertunga2003@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <cstdio>
#include "pico/stdlib.h"
#include "main.hpp"
#include "motor.h"

struct PidConstants {
    const float Kp{};
    const float Ki{};
    const float Kd{};
    const float integralMax{};
};

class Rover {
public:
    /**
     * @brief Construct a new Rover object that does not use PID algorithm.
     * 
     * @param motorSet The struct containing the pins used for motors.
     * @param servoSet The struct containing the pins used for servos.
     */
    Rover(const MotorSet& motorSet, const ServoSet& servoSet) : 
        motors(motorSet), servos(servoSet), pidFlag(false)
    { 

    }

    /**
     * @brief Construct a new Rover object that uses PID algorithm.
     * 
     * @param motorSet The struct containing the pins used for motors.
     * @param servoSet The struct containing the pins used for servos.
     * @param motorPid The struct containint the PID constants used for the motors.
     */
    Rover(const MotorSet& motorSet, const ServoSet& servoSet, const PidConstants& motorPid) : 
        motors(motorSet), servos(servoSet), pidFlag(true), kMotorPid(motorPid)
    { 

    }

    /**
     * @brief Set up the rover hardware.
     * 
     */
    void setup() {
        motors.frontLeft.setup();
        motors.frontRight.setup();
        motors.backLeft.setup();
        motors.backRight.setup();

        servos.frontLeft.setup();
        servos.frontRight.setup();
        servos.backLeft.setup();
        servos.backRight.setup();
    }

    /**
     * @brief Set the direction of the rover.
     * 
     * @param x The accepted values are between -100, 100. Positive means right direction, Negative means left direction.
     * @param y The accepted values are between -100, 100. Positive means forward, Negative means backward.
     */
    void setDirection(int x, int y) {
        
    }

    /**
     * @brief Set the speed of the rover.
     * 
     * @param speed The unit is in meters / second. If PID is not used, then there is no guarentee that the actual speed will be equal to the given input.
     */
    void setSpeed(float speedMs) {

    }

private:
    MotorSet motors{ {0}, {0}, {0}, {0}};
    ServoSet servos{ {0}, {0}, {0}, {0}};
    // EncoderSet encoders .....
    bool pidFlag { false };
    const PidConstants kMotorPid{ 0.0f, 0.0f , 0.0f, 0.0f };
};


int main() {
    
    stdio_init_all();

    constexpr uint motorFrontLeftPin { 1 };
    constexpr uint motorFrontRigthPin { 2 };
    constexpr uint motorBackLeftPin { 3 };
    constexpr uint motorBackRightPin { 4 };
    constexpr uint servoFrontLeftPin { 5 };
    constexpr uint servoFrontRigthPin { 6 };
    constexpr uint servoBackLeftPin { 7 };
    constexpr uint servoBackRightPin { 8 };

 
    gpio_init(25);
    gpio_set_dir(25, 1);
    gpio_put(25, 1);

    Rover rover(MotorSet(1, 2, 3, 4), ServoSet(5, 6, 7, 8));

    rover.setup();

    while(true) {

    }

    return 0;
}