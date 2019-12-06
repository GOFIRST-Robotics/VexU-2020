#include "tray.h"

Tray::Tray(std::shared_ptr<okapi::MotorGroup> motor) : System("Tray") {
    this->motor = motor;
    this->motor->setEncoderUnits(okapi::AbstractMotor::encoderUnits::degrees);
    this->motor->setGearing(okapi::AbstractMotor::gearset::red);
    this->motor->tarePosition();
    this->motor->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
    this->almpc = okapi::AsyncMotionProfileControllerBuilder()
                         .withOutput(*motor, 1_in, okapi::AbstractMotor::GearsetRatioPair(motor->getGearing(), 1))
                         .withLimits(okapi::PathfinderLimits({1, 1, 1}))
                         .buildLinearMotionProfileController();
}

void Tray::setAngle(okapi::QAngle target) {
    this->motor->moveAbsolute((target * 15).convert(1_deg), 75);
    
    /*if (this->target == target) {
        return;
    } 
    this->target = target;
    this->almpc->forceRemovePath("_");
    const okapi::RQuantity ratio = 1_in / 1_deg; // Transform angle into length
    okapi::QAngle curPos = this->motor->getPosition() * 1_deg / 15;
    this->almpc->generatePath({curPos * ratio, target * ratio}, "_");
    this->almpc->setTarget("_", target < curPos);
    this->almpc->flipDisable(false);*/
}

void Tray::waitUntilSettled() {
    this->almpc->waitUntilSettled();
}

void Tray::loop() {};