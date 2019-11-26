#include "tray.h"

Tray::Tray(std::shared_ptr<okapi::MotorGroup> motor) : System("Tray") {
    this->motor = motor;
    this->motor->setEncoderUnits(okapi::AbstractMotor::encoderUnits::degrees);
    this->almpc = okapi::AsyncMotionProfileControllerBuilder()
                         .withOutput(*motor, 1_in, motor->getGearing())
                         .withLimits(okapi::PathfinderLimits({1, 1, 1}))
                         .buildLinearMotionProfileController();
}

void Tray::setAngle(okapi::QAngle target) {
    const okapi::RQuantity ratio = 1_in / 1_deg; // Transform angle into length
    // This isn't a perfect 4-bar, so this might need adjusting
    okapi::QAngle curPos = this->motor->getPosition() * 1_deg;
    this->almpc->moveTo(curPos * ratio, target * ratio);
    this->almpc->flipDisable(false);
}

void Tray::waitUntilSettled() {
    this->almpc->waitUntilSettled();
}

void Tray::loop() {};