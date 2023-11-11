/**
 * @file servo.cpp
 * @author Alper Tunga Güven (alpertunga2003@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <cassert>
#include <cstdio>


#include "servo.h"

Servo::Servo(const uint servoPin)
    : mServoPin(servoPin), mSliceNum(pwm_gpio_to_slice_num(mServoPin)) {
  assert(servoPin <= 30);
}

void Servo::setBoundsUs(uint min, uint max) {
  assert(max > min && "Servo Bounds Logic Error\n");
  mMinUs = min;
  mMaxUs = max;
}
void Servo::setBoundsDegree(float min, float max) {
  assert(max > min && "Servo Bounds Logic Error\n");
  mMinDegree = min;
  mMaxDegree = max;
}

void Servo::setup() {

  gpio_init(mServoPin);
  gpio_set_dir(mServoPin, GPIO_OUT);
  gpio_set_function(mServoPin, GPIO_FUNC_PWM);

  const uint sliceNum{pwm_gpio_to_slice_num(mServoPin)};

  pwm_config config{pwm_get_default_config()};

  uint32_t clockSpeed = clock_get_hz(clk_sys);

  const float clockDiv{ static_cast<float>(clockSpeed / 100000U) }; // For 1us clock period.
  wrap = (clockSpeed / clockDiv) * mPeriodUs; // For 1us precision between the ranges 0-2400us.

  pwm_config_set_clkdiv(&config, clockDiv);
  pwm_config_set_wrap(&config, wrap);

  pwm_init(sliceNum, &config, false);

  if (debugFlag) {
    printf("Debug Messeage => Servo %u Init with ClockSpeed: %u, ClockDiv: %u "
           "and Wrap: %u\n",
           mServoPin, clockSpeed, clockDiv, wrap);
  }
}

void Servo::enable() {
  pwm_set_enabled(mSliceNum, true);
  if (debugFlag) {
    printf("Debug Messeage => Servo %u Enabled\n", mServoPin);
  }
}

void Servo::setUs(const uint micros) {
  uint16_t chanLevel{ static_cast<uint16_t>(micros)};
  assert(chanLevel >= mMinUs && chanLevel <= mMaxUs &&
         "Channel Level is not within bounds\n");
  pwm_set_chan_level(mSliceNum, mServoPin % 2, chanLevel);
  if (debugFlag) {
    printf("Debug Messeage => Servo %u Set with chanLevel: %u\n", mServoPin,
           chanLevel);
  }
}

void Servo::setDegree(const float degree) {
  float multiplier{static_cast<float>(mMaxUs - mMinUs) /
                   (mMaxDegree - mMinDegree)};
  uint16_t chanLevel{static_cast<uint16_t>(degree * multiplier)};
  assert(chanLevel >= mMinUs && chanLevel <= mMaxUs &&
         "Channel Level is not within bounds\n");
  printf("Debug Messeage => Servo %u Set with chanLevel: %u\n", mServoPin,
         chanLevel);
  if (debugFlag) {
    printf("Debug Messeage => Servo %u Set with chanLevel: %u\n", mServoPin,
           chanLevel);
  }
}

void Servo::deInit() {
  pwm_set_enabled(mSliceNum, false);
  gpio_deinit(mServoPin);
}

Servo::~Servo() { deInit(); }
