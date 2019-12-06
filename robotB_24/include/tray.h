#pragma once
#include "gopher/system.h"

class Tray : public System {
    public:
    Tray(std::shared_ptr<okapi::MotorGroup> motor);
    void setAngle(okapi::QAngle target);
    void waitUntilSettled();

    private:
    okapi::QAngle target = 0_deg;
    std::shared_ptr<okapi::MotorGroup> motor;
    std::shared_ptr<okapi::AsyncLinearMotionProfileController> almpc;
    void loop();
};