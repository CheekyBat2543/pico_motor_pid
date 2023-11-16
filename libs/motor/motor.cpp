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

void PwmController::setBoundsUs(uint min, uint max)
{
    mMinUs = min;
    mMaxUs = max;
}

void PwmController::setup()
{
    gpio_set_function(mPwmPin, GPIO_FUNC_PWM);

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

void PwmController::setUs(const uint micros)
{
    /* Calculate the pwm "on" period. */
    uint16_t chanLevel{static_cast<uint16_t>((micros * mWrap) / mPeriodUs)};
    /* Check if the needed channel level is within the accepted range. */
    chanLevel = chanLevel > mMaxUs ? mMaxUs : chanLevel;
    chanLevel = chanLevel < mMinUs ? mMinUs : chanLevel;
    pwm_set_chan_level(mSliceNum, mPwmPin % 2, chanLevel);
    if (debugFlag)
    {
        printf("Debug Messeage => Motor %u Set with chanLevel: %u\n", mPwmPin,
               chanLevel);
    }
}

void PwmController::setDutyCycle(const float dutyCycle)
{
    float multiplier{static_cast<float>(mMaxUs) / 100.0f};
    printf("Duty Cycle = %.2f , Multiplier = %.2f\n", dutyCycle, multiplier);
    PwmController::setUs(static_cast<uint16_t>(dutyCycle * multiplier));
}

void PwmController::setPeriod(const uint us)
{
    mPeriodUs = us;
    PwmController::setup();
}

void PwmController::deInit()
{
    pwm_set_enabled(mSliceNum, false);
    gpio_deinit(mPwmPin);
}

PwmController::~PwmController() { deInit(); }

Motor::Motor(const uint motorPin) : PwmController(motorPin) {
    mMinUs = 0;
    mMaxUs = kMotorShieldHz * kMaxDutyRate;
    mPeriodUs = kMotorShieldHz;
}

Motor::Motor(const uint motorPin, const uint pwmPeriod) : PwmController(motorPin) {
    mMinUs = 0;
    mMaxUs = pwmPeriod * kMaxDutyRate;
    mPeriodUs = pwmPeriod;
}

Servo::Servo(const uint servoPin) : PwmController(servoPin)
{
    mMinUs = 600U;
    mMaxUs = 2400U;
    mPeriodUs = kServoPeriod;
}

Servo::Servo(const uint servoPin, const int minDegree, const int maxDegree)
    : PwmController(servoPin), mMinDegree(minDegree), mMaxDegree(maxDegree)
{
    mMinUs = 600U;
    mMaxUs = 2400U;
    mPeriodUs = kServoPeriod;
}

void Servo::setDegree(const float degree)
{
    float multiplier{static_cast<float>(mMaxUs - mMinUs) /
                     (mMaxDegree - mMinDegree)};
    PwmController::setUs(static_cast<uint>(degree * multiplier + ((mMaxUs - mMinUs) / 2)));
}
