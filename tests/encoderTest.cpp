#include "pico/stdlib.h"
#include "encoder.h"
#include <cstdio>

int main() {

    stdio_init_all();

    constexpr uint gpio {15};

    gpio_init(gpio);
    gpio_set_dir(gpio, 0);
    gpio_pull_down(gpio);

    Encoder::setup();

    while(true) {
        int32_t rpmValues[Encoder::kMaxSize] {};
        Encoder::getRpm(rpmValues);
        printf("Rpm Values: 1 = %d, 2 = %d, 3 = %d, 4 = %d", rpmValues[0], rpmValues[1], rpmValues[2], rpmValues[3]);
        sleep_ms(100);
    }

}