#ifndef PICO_ENCODER_H
#define PICO_ENCODER_H

#include "pico/stdlib.h"

namespace Encoder {
    
    enum Position {
        frontLeft,
        frontRight,
        backLeft,
        backRight,
        kMaxSize,
    };

    static repeating_timer timer{};

    constexpr uint frontLeftPin { 0 };
    constexpr uint frontRightPin { 1 };
    constexpr uint backLeftPin { 3 };
    constexpr uint backRightPin { 2 };
    constexpr uint pins[kMaxSize] { frontLeftPin, frontRightPin, backLeftPin, backRightPin };

    constexpr uint kPulsesPerRotation { 64 };
    constexpr uint kPulseReadPeriodUs { 100000 };

    uint64_t counter[kMaxSize] {};
    uint64_t prevCounter[kMaxSize] {};
    uint32_t rpm[kMaxSize] {};

    void gpioCallback(uint gpio, uint32_t flags) {
        switch(gpio) {
            case pins[frontLeftPin]:
                counter[frontLeftPin]++;
                return;
            case pins[frontRightPin]:
                counter[frontRightPin]++;
                return;
            case pins[backLeftPin]:
                counter[backLeftPin]++;
                return;
            case pins[backRightPin]:
                counter[backRightPin]++;
                return;
            default:
                return;
        }
    }

    bool timerCallback(repeating_timer_t *rt) {
        constexpr float kMultiplier { (1000000.0f / (kPulseReadPeriodUs * kPulsesPerRotation) * 60) };
        for(int i = 0; i < kMaxSize; i++) {
            const uint32_t counterDifference { static_cast<uint32_t>(counter[i] - prevCounter[i]) };
            rpm[i] = static_cast<uint32_t>(counterDifference * kMultiplier);
            prevCounter[i] = counter[i];
        }
        return true;
    }

    bool setup() {

        gpio_set_irq_enabled_with_callback(pins[0], GPIO_IRQ_EDGE_RISE, true, Encoder::gpioCallback);
        for(int i = 1; i < kMaxSize; i++) {
            gpio_set_irq_enabled(pins[i], GPIO_IRQ_EDGE_RISE, true);
        }
        if (!add_repeating_timer_us(Encoder::kPulseReadPeriodUs, Encoder::timerCallback, NULL, &timer)) {
                return false;
        }  
        return true;        
    }

    void getRpm(uint32_t * rpmArray) {
        for(int i = 0; i < kMaxSize; i++) {
            rpmArray[i] = rpm[i];
        }
    }

}


#endif