#include "intake.h"

Intake::Intake(std::shared_ptr<okapi::MotorGroup> left, std::shared_ptr<okapi::MotorGroup> right) : System("Intake") {
    this->left = left;
    this->right = right;
}

void Intake::idle() {
    this->state = Intake::State::Idle;
}

void Intake::intake() {
    this->state = Intake::State::Intaking;
}

void Intake::reverse() {
    this->state = Intake::State::Reverse;
}


void Intake::loop() {
    const int intake_speed = 300;
    const int reverse_speed = -300;
    switch (this->state) {
        case Intake::State::Idle:
        this->left->moveVelocity(0);
        this->right->moveVelocity(0);
        break;
        case Intake::State::Intaking:
        this->left->moveVelocity(intake_speed);
        this->right->moveVelocity(intake_speed);
        break;
        case Intake::State::Reverse:
        this->left->moveVelocity(reverse_speed);
        this->right->moveVelocity(reverse_speed);
        break;
    }
};