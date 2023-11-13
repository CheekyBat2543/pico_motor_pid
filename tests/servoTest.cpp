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
        printf("Enter pulse length in microseconds: ");
        uint micros {};
        bool status = scanf("%u", &micros);
        printf("Entered pulse length: %u", micros);
        if(status) {
            servo.setUs(micros);
        }
        sleep_ms(200);
    }
    return 0;
}