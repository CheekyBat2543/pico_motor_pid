/**
 * @file servo.h
 * @author Alper Tunga Güven (alpertunga2003@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SERVO_H
#define SERVO_H


class Servo{
public:
    /**
     * @brief Construct a new Servo object
     * 
     * @param servoPin 
     */
    Servo(const uint servoPin);
    
    /**
     * @brief Set the Bounds of the PWM pulse in Microseconds.
     * 
     * @param min 
     * @param max 
     */
    void setBoundsUs(uint min, uint max);

    /**
     * @brief Set the Bounds of the servo in degrees.
     * 
     * @param min 
     * @param max 
     */
    void setBoundsDegree(float min, float max);

    /**
     * @brief Setup the PWM configuration necessary to drive the servo.
     * 
     */
    void setup();

    /**
     * @brief Enable the PWM pin to start operating.
     * 
     */
    void enable();

    /**
     * @brief Set the length of the PWM pulse in microseconds.
     * 
     * @param micros 
     */
    void setUs(const uint micros);

    /**
     * @brief Set the Degree of the servo motor.
     * 
     * @param degree 
     */
    void setDegree(const float degree);

    /**
     * @brief Set the Period of the PWM signal that the servo needs.
     * 
     * @param us 
     */
    void setPeriod(const uint us);

    /**
     * @brief Deinitiliaze the servo pin used.
     * 
     */
    void deInit();

    ~Servo();

private:
    const uint mServoPin{};
    const uint mSliceNum{};
    uint mPeriodUs { 20000 };
    uint mWrap { 39062 };
    uint mMinUs{ 600 };
    uint mMaxUs{ 2400 };
    int mMaxDegree { 90 };
    int mMinDegree {-90 };
    #ifdef NDEBUG
    static constexpr bool debugFlag{ false };
    #else
    static constexpr bool debugFlag{ true };
    #endif
};


#endif