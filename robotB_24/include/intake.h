#pragma once
#include "gopher/system.h"

class Intake : public System {
    public:
    Intake(std::shared_ptr<okapi::MotorGroup> left, std::shared_ptr<okapi::MotorGroup> right);
    void intake();
    void idle();
    void reverse();

    enum State {
        Idle = 0,
        Intaking,
        Reverse
    };

    private:
    Intake::State state = Intake::State::Idle;
    std::shared_ptr<okapi::MotorGroup> left;
    std::shared_ptr<okapi::MotorGroup> right;
    void loop();
};