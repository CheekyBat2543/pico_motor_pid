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

constexpr uint servoPin { 0 };
constexpr uint motorPin { 5 };

class Rover {
public:

    Rover(const MotorSet& motorSet, const ServoSet& servoSet) : motors(motorSet), servos(servoSet)
    { 

    }

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

    void setDirection(int forward, int backward, int left, int right) {

    }

    void setSpeed(float speed) {

    }

    void pidEnable(bool enable) {

    }

private:
    MotorSet motors{ {0}, {0}, {0}, {0}};
    ServoSet servos{ {0}, {0}, {0}, {0}};
    // EncoderSet encoders .....
    bool pidFlag { false };
};


int main() {
    
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, 1);
    gpio_put(25, 1);

    Rover rover(MotorSet(1, 2, 3, 4), ServoSet(5, 6, 7, 8));

    rover.setup();

    while(true) {

    }

    return 0;
}