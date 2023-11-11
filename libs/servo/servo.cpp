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

  gpio_set_function(mServoPin, GPIO_FUNC_PWM);

  pwm_config config{pwm_get_default_config()};

  uint32_t clockSpeed = clock_get_hz(clk_sys);

  const float clockDiv{ static_cast<float>(clockSpeed / 1000000U) }; // For 1us clock period.
  mWrap = (clockSpeed / clockDiv) * (static_cast<float>(mPeriodUs) / 1000000.0f); // For 1us precision between the ranges 0-2400us.
  pwm_config_set_clkdiv(&config, clockDiv);
  pwm_config_set_wrap(&config, mWrap);

  pwm_init(mSliceNum, &config, true);

  if (debugFlag) {
    printf("Debug Messeage => Servo %u Init with ClockSpeed: %u, ClockDiv: %u "
           "and Wrap: %u\n",
           mServoPin, clockSpeed, clockDiv, mWrap);
  }
}

void Servo::enable() {
  pwm_set_enabled(mSliceNum, true);
  if (debugFlag) {
    printf("Debug Messeage => Servo %u Enabled\n", mServoPin);
  }
}

void Servo::setUs(const uint micros) {
  uint16_t chanLevel{ static_cast<uint16_t>(micros * mWrap / mPeriodUs)};
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
  Servo::setUs(static_cast<uint>(degree * multiplier));
}

void Servo::setPeriod(const uint us) {
  mPeriodUs = us;
  Servo::setup();
}

void Servo::deInit() {
  pwm_set_enabled(mSliceNum, false);
  gpio_deinit(mServoPin);
}

Servo::~Servo() { deInit(); }
