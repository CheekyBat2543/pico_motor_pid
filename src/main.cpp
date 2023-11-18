/**
 * @file main.cpp
 * @author Alper Tunga Güven (alpertunga2003@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <cstdio>
#include <cmath>
#include "pico/stdlib.h"
#include "main.hpp"
#include "motor.h"
#include "encoder.h"

struct PidConstants {
    const float Kp{};
    const float Ki{};
    const float Kd{};
    const int32_t integralMax{};
};

class Rover {
public:
    /**
     * @brief Construct a new Rover object that does not use PID algorithm.
     * 
     * @param motorSet The struct containing the pins used for motors.
     * @param servoSet The struct containing the pins used for servos.
     */
    Rover(const MotorSet& motorSet, const ServoSet& servoSet) : 
        mMotors(motorSet), mServos(servoSet), mPidFlag(false)
    { 

    }

    /**
     * @brief Construct a new Rover object that uses PID algorithm.
     * 
     * @param motorSet The struct containing the pins used for motors.
     * @param servoSet The struct containing the pins used for servos.
     * @param motorPid The struct containint the PID constants used for the motors.
     */
    Rover(const MotorSet& motorSet, const ServoSet& servoSet, const PidConstants& motorPid) : 
        mMotors(motorSet), mServos(servoSet), mPidFlag(true), kMotorPid(motorPid)
    { 

    }

    /**
     * @brief Set up the rover hardware.
     * 
     */
    void setup() {
        Encoder::setup();

        mMotors.frontLeft.setup();
        mMotors.frontRight.setup();
        mMotors.backLeft.setup();
        mMotors.backRight.setup();

        mServos.frontLeft.setup();
        mServos.frontRight.setup();
        mServos.backLeft.setup();
        mServos.backRight.setup();
    }

    /**
     * @brief Set the direction of the rover.
     * 
     * @param x The accepted values are between -100, 100. Positive means right direction, Negative means left direction.
     * @param y The accepted values are between -100, 100. Positive means forward, Negative means backward.
     */
    void setDirection(int x, int y) {
        
    }

    /**
     * @brief Set the speed of the rover.
     * 
     * @param speed The unit is in meters / second. If PID is not used, then there is no guarentee that the actual speed will be equal to the given input.
     */
    void setSpeed(float speedMs) {
        int32_t targetRpm { msToRpm(speedMs) };
        int32_t pid[Encoder::kMaxSize] {};
        uint32_t motorPwm[Encoder::kMaxSize] {};
        bool direction { targetRpm >= 0 };
        Encoder::getRpm(mCurrentRpm);
        if(mPidFlag) {
            for(int i = 0; i < Encoder::kMaxSize; i++) {
                pid[i] = calculatePid(targetRpm, mCurrentRpm[i]);
                motorPwm[i] = abs(pid[i]);
            }
        } else {
            for(int i = 0; i < Encoder::kMaxSize; i++) {
                motorPwm[i] = (mMotors.frontLeft.getMaxNs() * mCurrentRpm[i]) / msToRpm(kMaxSpeed);
            }
        }
    }

private:
    MotorSet mMotors{ {0}, {0}, {0}, {0}};
    ServoSet mServos{ {0}, {0}, {0}, {0}};
    bool mPidFlag { false };
    int32_t mCurrentRpm[Encoder::kMaxSize] {};
    int32_t mPrevRpm[Encoder::kMaxSize] {};
    static constexpr float kWheelRadiusCm { 5.0 };
    static constexpr float kPi { 3.14159265359 };
    static constexpr float kMaxSpeed { 4.166f };
    int32_t mIntegral { 0 };
    absolute_time_t mPrevTime {};
    const PidConstants kMotorPid{ 0.0f, 0.0f , 0.0f, 0 };

    float rpmToMs(int32_t rpm) {
        constexpr float multiplier { (2 * (kWheelRadiusCm / 100.f)* kPi) / 60.0f };
        const float ms { static_cast<float>(rpm) * multiplier };  
        return ms; 
    }
    int32_t msToRpm(float ms) {        
        constexpr float multiplier { 60.0f / (2 * (kWheelRadiusCm / 100.f)* kPi) };
        const uint32_t rpm { static_cast<uint32_t>(ms * multiplier) };  
        return rpm; 
    }

    int32_t calculatePid(int32_t targetRpm, int32_t currentRpm) {
        const int32_t error { targetRpm - currentRpm };

        const int32_t proportional { static_cast<int32_t>(error * kMotorPid.Kp) };

        mIntegral += static_cast<int32_t>(error * kMotorPid.Ki);
        if(mIntegral >= kMotorPid.integralMax) 
            mIntegral = kMotorPid.integralMax;
        else if(mIntegral <= (-1* kMotorPid.integralMax)) {
            mIntegral = (-1 * kMotorPid.integralMax);
        }

        const auto currentTime { get_absolute_time() };
        const int64_t deltaT { absolute_time_diff_us(mPrevTime, currentTime) };
        const int32_t derivative { static_cast<int32_t>(error * kMotorPid.Kd * (1000000.f / deltaT)) };

        const int32_t pid { proportional + derivative + mIntegral };
        return pid;
    }

};


int main() {
    
    stdio_init_all();

    constexpr uint motorFrontLeftPin { 1 };
    constexpr uint motorFrontRigthPin { 2 };
    constexpr uint motorBackLeftPin { 3 };
    constexpr uint motorBackRightPin { 4 };
    constexpr uint servoFrontLeftPin { 5 };
    constexpr uint servoFrontRigthPin { 6 };
    constexpr uint servoBackLeftPin { 7 };
    constexpr uint servoBackRightPin { 8 };

 
    gpio_init(25);
    gpio_set_dir(25, 1);
    gpio_put(25, 1);

    Rover rover(MotorSet(1, 2, 3, 4), ServoSet(5, 6, 7, 8));

    rover.setup();

    while(true) {

    }

    return 0;
}