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
#include <cstdio>

#include "motor.h"

Motor::Motor(const uint motorPin)
    : mMotorPin(motorPin), mSliceNum(pwm_gpio_to_slice_num(mMotorPin)) {}

void Motor::setBoundsUs(uint min, uint max)
{
    mMinUs = min;
    mMaxUs = max;
}

void Motor::setup()
{
    gpio_set_function(mMotorPin, GPIO_FUNC_PWM);

    pwm_config config{pwm_get_default_config()};

    uint32_t clockSpeed = clock_get_hz(clk_sys);

    const float clockDiv{
        static_cast<float>(clockSpeed / 1000000U)}; // For 1us clock period.
    mWrap = (clockSpeed / clockDiv * static_cast<float>(mPeriodUs)) / 1000000.0f;

    pwm_config_set_clkdiv(&config, clockDiv);
    pwm_config_set_wrap(&config, mWrap);

    pwm_init(mSliceNum, &config, true);

    if (debugFlag)
    {
        printf("Debug Messeage => Motor %u Init with ClockSpeed: %u, ClockDiv: %.2f "
               "and mWrap: %u\n",
               mMotorPin, clockSpeed, clockDiv, mWrap);
    }
}

void Motor::enable()
{
    pwm_set_enabled(mSliceNum, true);
    if (debugFlag)
    {
        printf("Debug Messeage => Motor %u Enabled\n", mMotorPin);
    }
}

void Motor::setUs(const uint micros)
{
    uint16_t chanLevel{static_cast<uint16_t>((micros * mWrap) / mPeriodUs)};
    if (debugFlag)
    {
        chanLevel = chanLevel > mMaxUs ? mMaxUs : chanLevel;
        chanLevel = chanLevel < mMinUs ? mMinUs : chanLevel;
    }
    pwm_set_chan_level(mSliceNum, mMotorPin % 2, chanLevel);
    if (debugFlag)
    {
        printf("Debug Messeage => Motor %u Set with chanLevel: %u\n", mMotorPin,
               chanLevel);
    }
}

void Motor::setDutyCycle(const float dutyCycle)
{
    float multiplier{static_cast<float>(mPeriodUs) / 100.0f};
    Motor::setUs(static_cast<uint16_t>(dutyCycle * multiplier));
}

void Motor::setPeriod(const uint us)
{
    mPeriodUs = us;
    Motor::setup();
}

void Motor::deInit()
{
    pwm_set_enabled(mSliceNum, false);
    gpio_deinit(mMotorPin);
}

Motor::~Motor() { deInit(); }

Servo::Servo(const uint servoPin) : Motor(servoPin)
{
    mMinUs = 600U;
    mMaxUs = 2400U;
    mPeriodUs = 20000U;
}

Servo::Servo(const uint servoPin, const int minDegree, const int maxDegree)
    : Motor(servoPin), mMinDegree(minDegree), mMaxDegree(maxDegree)
{
    mMinUs = 600U;
    mMaxUs = 2400U;
    mPeriodUs = 20000U;
}

void Servo::setDegree(const float degree)
{
    float multiplier{static_cast<float>(mMaxUs - mMinUs) /
                     (mMaxDegree - mMinDegree)};
    Servo::setUs(static_cast<uint>(degree * multiplier + mMinUs));
}
