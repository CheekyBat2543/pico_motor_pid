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

#include "pico/stdlib.h"
#include "motor.h"
#include <cstdio>

constexpr uint servoPin { 0 };
constexpr uint motorPin { 5 };

int main() {
    
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, 1);
    gpio_put(25, 1);

    Servo servo(servoPin);
    Motor motor(motorPin);

    // servo.setup();
    motor.setup();

    while(true) {
        servo.setUs(1000);
        sleep_ms(1000);
        servo.setUs(1500);
        sleep_ms(2000);
        servo.setUs(2000);
        sleep_ms(2000);
        printf("Bruh!\n");
    }
    return 0;
}