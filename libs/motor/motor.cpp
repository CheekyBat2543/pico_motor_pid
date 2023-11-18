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

PwmController::PwmController(const uint pwmPin)
    : mPwmPin(pwmPin), mSliceNum(pwm_gpio_to_slice_num(mPwmPin)) {}

void PwmController::setBoundsNs(const uint32_t min, const uint32_t max)
{
    mMinNs = min;
    mMaxNs = max;
}

void PwmController::setup()
{
    gpio_set_function(mPwmPin, GPIO_FUNC_PWM);

    pwm_config config{pwm_get_default_config()};

    uint32_t clockSpeed = clock_get_hz(clk_sys);

    float clockDiv{1.0f};
    if (mPeriodNs >= 1000000) {
        clockDiv = static_cast<float>(clockSpeed / 1000000U); // For 1us clock cycle.
        mWrap = (clockSpeed / clockDiv * static_cast<float>(mPeriodNs)) / 1000000000.0f;
    } else {
        mWrap = (clockSpeed / clockDiv * static_cast<float>(mPeriodNs)) / 1000000000.0f;
    }
 
    pwm_config_set_clkdiv(&config, clockDiv);
    pwm_config_set_wrap(&config, mWrap);

    pwm_init(mSliceNum, &config, true);

    if (debugFlag)
    {
        printf(
            "Debug Messeage => Motor %u Init with ClockSpeed: %u, ClockDiv: %.2f "
            "and mWrap: %u\n",
            mPwmPin, clockSpeed, clockDiv, mWrap);
    }
}

void PwmController::enable()
{
    pwm_set_enabled(mSliceNum, true);
    if (debugFlag)
    {
        printf("Debug Messeage => Motor %u Enabled\n", mPwmPin);
    }
}

void PwmController::setNs(const uint32_t nanos)
{
    /* Calculate the pwm "on" period. */
    uint16_t chanLevel{static_cast<uint16_t>((nanos * mWrap) / mPeriodNs)};
    /* Check if the needed channel level is within the accepted range. */
    chanLevel = chanLevel > mMaxNs ? mMaxNs : chanLevel;
    chanLevel = chanLevel < mMinNs ? mMinNs : chanLevel;
    pwm_set_chan_level(mSliceNum, mPwmPin % 2, chanLevel);
    if (debugFlag)
    {
        printf("Debug Messeage => Motor %u Set with chanLevel: %u\n", mPwmPin,
               chanLevel);
    }
}

void PwmController::setDutyCycle(const float dutyCycle)
{
    float multiplier{static_cast<float>(mMaxNs) / 100.0f};
    printf("Duty Cycle = %.2f , Multiplier = %.2f\n", dutyCycle, multiplier);
    PwmController::setNs(static_cast<uint16_t>(dutyCycle * multiplier));
}

void PwmController::setFrequency(const uint32_t hz)
{
    mFrequency = hz;
    mPeriodNs = static_cast<uint32_t>(1000000000.0f / mFrequency);
    PwmController::setup();
}

void PwmController::setPeriodNs(const uint32_t ns)
{
    mPeriodNs = ns;
    mFrequency = static_cast<uint32_t>(1000000000.0f / mPeriodNs);
    PwmController::setup();
}

uint16_t PwmController::getMaxNs() {
    return mMaxNs;
}


void PwmController::deInit()
{
    pwm_set_enabled(mSliceNum, false);
    gpio_deinit(mPwmPin);
}

PwmController::~PwmController() { deInit(); }

Motor::Motor(const uint motorPin) : PwmController(motorPin)
{
    mMinNs = 0;
    mMaxNs = kMotorShieldPeriodNs * kMaxDutyRate;
    mPeriodNs = kMotorShieldPeriodNs;
}

Motor::Motor(const uint motorPin, const uint32_t pwmPeriodNs)
    : PwmController(motorPin)
{
    mMinNs = 0;
    mMaxNs = pwmPeriodNs * kMaxDutyRate;
    mFrequency = 1000000000 / pwmPeriodNs;
    mPeriodNs = pwmPeriodNs;
}

Servo::Servo(const uint servoPin) : PwmController(servoPin)
{
    mMinNs = 600000U;
    mMaxNs = 2400000U;
    mPeriodNs = kServoPeriodNs;
}

Servo::Servo(const uint servoPin, const int minDegree, const int maxDegree)
    : PwmController(servoPin), mMinDegree(minDegree), mMaxDegree(maxDegree)
{
    mMinNs = 600000U;
    mMaxNs = 2400000U;
    mPeriodNs = kServoPeriodNs;
}

void Servo::setDegree(const float degree)
{
    float multiplier{static_cast<float>(mMaxNs - mMinNs) /
                     (mMaxDegree - mMinDegree)};
    PwmController::setNs(
        static_cast<uint>(degree * multiplier + ((mMaxNs - mMinNs) / 2)));
}
