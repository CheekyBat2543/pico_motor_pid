/**
 * @file motor.h
 * @author Alper Tunga Güven (alpertunga2003@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
public:
    Motor(const uint motorPin);

    void setBoundsUs(uint min, uint max);

    void setup();

    void enable();

    void setUs(const uint micros);

    void deInit();

    void setPeriod(const uint us);

    void setDutyCycle(const float dutyCycle);

    ~Motor();

protected:
    const uint mMotorPin{};
    const uint mSliceNum{};
    uint16_t mWrap{};
    uint mMinUs{ 0 };
    uint mMaxUs{ kMotorShieldHz };
    uint mPeriodUs{ kMotorShieldHz };
    static constexpr uint kMotorShieldHz { 16000 };
#ifdef NDEBUG
    static constexpr bool debugFlag{false};
#else
    static constexpr bool debugFlag{true};
#endif
};



class Servo : public Motor
{
public:
    Servo(const uint servoPin);

    Servo(const uint servoPin, const int minDegree, const int maxDegree);

    void setDegree(const float degree);

private:
    int mMinDegree{-90};
    int mMaxDegree{90};
};

#endif