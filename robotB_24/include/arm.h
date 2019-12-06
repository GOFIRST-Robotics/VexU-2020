#pragma once
#include "gopher/system.h"

class Arm : public System {
    public:
    Arm(std::shared_ptr<okapi::MotorGroup> motor);
    void setAngle(okapi::QAngle target);
    void movePower(float power);
    void waitUntilSettled();

    private:
    okapi::QAngle target = 0_deg;
    std::shared_ptr<okapi::MotorGroup> motor;
    std::shared_ptr<okapi::AsyncLinearMotionProfileController> almpc;
    void loop();
};