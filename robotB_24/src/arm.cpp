#include "arm.h"

Arm::Arm(std::shared_ptr<okapi::MotorGroup> motor) : System("Arm") {
    this->motor = motor;
    this->motor->setEncoderUnits(okapi::AbstractMotor::encoderUnits::degrees);
    this->almpc = okapi::AsyncMotionProfileControllerBuilder()
                         .withOutput(*motor, 1_in, motor->getGearing())
                         .withLimits(okapi::PathfinderLimits({1, 1, 1}))
                         .buildLinearMotionProfileController();
}

void Arm::setAngle(okapi::QAngle target) {
    const okapi::RQuantity ratio = 1_in / 1_deg; // Transform angle into length
    okapi::QAngle curPos = this->motor->getPosition() * 1_deg;
    this->almpc->moveTo(curPos * ratio, target * ratio);
    this->almpc->flipDisable(false);
}

void Arm::waitUntilSettled() {
    this->almpc->waitUntilSettled();
}

void Arm::loop() {};