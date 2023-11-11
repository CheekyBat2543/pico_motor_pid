/**
 * @file motor.cpp
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
#include <stdio.h>

#include "motor.h"

Motor::Motor(const uint motorPin)
    : mMotorPin(motorPin), mSliceNum(pwm_gpio_to_slice_num(mMotorPin)) {
  assert(motorPin <= 30 && "Motor Pin not valid!\n");
}

void Motor::setBoundsUs(uint min, uint max) {
  assert(max > min && "Motor Bounds Logic Error\n");
  mMinUs = min;
  mMaxUs = max;
}

void Motor::setup() {

  gpio_init(mMotorPin);
  gpio_set_dir(mMotorPin, GPIO_OUT);
  gpio_set_function(mMotorPin, GPIO_FUNC_PWM);

  const uint sliceNum{pwm_gpio_to_slice_num(mMotorPin)};

  pwm_config config{pwm_get_default_config()};

  uint32_t clockSpeed = clock_get_hz(clk_sys);

  const float clockDiv{ static_cast<float>(clockSpeed / 1000000U) }; // For 1us clock period.
  mWrap = (clockSpeed / clockDiv * static_cast<float>(mMaxUs)) / 1000000;

  pwm_config_set_clkdiv(&config, clockDiv);
  pwm_config_set_wrap(&config, mWrap);

  pwm_init(sliceNum, &config, false);

  if (debugFlag) {
    printf("Debug Messeage => Motor %u Init with ClockSpeed: %u, ClockDiv: %u "
           "and mWrap: %u\n",
           mMotorPin, clockSpeed, clockDiv, mWrap);
  }
}

void Motor::enable() {
  pwm_set_enabled(mSliceNum, true);
  if (debugFlag) {
    printf("Debug Messeage => Motor %u Enabled\n", mMotorPin);
  }
}

void Motor::setUs(const uint micros) {
  uint16_t chanLevel{static_cast<uint16_t>((micros * mWrap) / mMaxUs)};
  assert(chanLevel >= mMinUs && chanLevel <= mMaxUs &&
         "Channel Level is not within bounds\n");
  pwm_set_chan_level(mSliceNum, mMotorPin % 2, chanLevel);
  if (debugFlag) {
    printf("Debug Messeage => Motor %u Set with chanLevel: %u\n", mMotorPin,
           chanLevel);
  }
}

void Motor::setDutyCycle(const float dutyCycle) {
  assert(dutyCycle >= 0.0f && dutyCycle <= 100.0f &&
         "Duty Cycle out of Range\n");
  float multiplier{static_cast<float>(mMaxUs - mMinUs) / 100.0f};
  Motor::setUs(static_cast<uint16_t>(dutyCycle * multiplier));
  
}

void Motor::deInit() {
  pwm_set_enabled(mSliceNum, false);
  gpio_deinit(mMotorPin);
}

Motor::~Motor() { deInit(); }