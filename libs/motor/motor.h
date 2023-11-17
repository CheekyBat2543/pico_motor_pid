/**
 * @file motor.h
 * @author Alper Tunga Güven (alpertunga2003@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 * 
 * Motor Shield Link: https://www.amazon.com.tr/Akozon-DC5-12V-0A-30A-Dual-channel-Arduino/dp/B07H2MDXMN/ref=asc_df_B07H2MDXMN/?tag=trshpngglede-21&linkCode=df0&hvadid=510499475756&hvpos=&hvnetw=g&hvrand=11669362855173271918&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1012783&hvtargid=pla-703073782452&psc=1
 *
 */

#ifndef MOTOR_H
#define MOTOR_H

class PwmController {

public:

    PwmController(const uint pwmPin);

    void setBoundsNs(const uint32_t min, const uint32_t max);

    void setup();

    void enable();

    void setPeriodNs(const uint32_t ns);

    void setFrequency(const uint32_t hz);

    void setNs(const uint32_t nanos);

    void setDutyCycle(const float dutyCycle);

    void deInit();

    ~PwmController();
protected:
    const uint mPwmPin{};
    const uint mSliceNum{};
    uint16_t mWrap{};
    uint32_t mMinNs{ 0 };
    uint32_t mMaxNs{ 16000000 };
    uint32_t mPeriodNs{ mMaxNs };
    uint32_t mFrequency{ static_cast<uint32_t>(1000000000.0f / mPeriodNs) };
#ifdef NDEBUG
    static constexpr bool debugFlag{false};
#else
    static constexpr bool debugFlag{true};
#endif
};

/**
 * @brief A motor controller class designed to be used with:
 * 
 */
class Motor : public PwmController
{
public:
    Motor(const uint motorPin);

    Motor(const uint motorPin, const uint32_t pwmPeriod);

private:
    static constexpr uint32_t kMotorShieldHz { 16000 };
    static constexpr uint32_t kMotorShieldPeriodNs { 1000000000 / kMotorShieldHz };
    static constexpr float kMaxDutyRate{ 98.0f };
};



class Servo : public PwmController
{
public:
    Servo(const uint servoPin);

    Servo(const uint servoPin, const int minDegree, const int maxDegree);

    void setDegree(const float degree);

private:
    int mMinDegree{-90};
    int mMaxDegree{90};
    static constexpr uint32_t kServoPeriodNs{ 20000000 };
};

#endif