#include "arm.h"

Arm::Arm(std::shared_ptr<okapi::MotorGroup> motor) : System("Arm") {
    this->motor = motor;
    this->motor->setEncoderUnits(okapi::AbstractMotor::encoderUnits::degrees);
    this->motor->setGearing(okapi::AbstractMotor::gearset::red);
    this->motor->tarePosition();
    this->motor->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
    this->almpc = okapi::AsyncMotionProfileControllerBuilder()
                         .withOutput(*motor, 1_in, okapi::AbstractMotor::GearsetRatioPair(motor->getGearing(), 60/12))
                         .withLimits(okapi::PathfinderLimits({3, 1, 1}))
                         .buildLinearMotionProfileController();
}

void Arm::setAngle(okapi::QAngle target) {
    this->motor->moveAbsolute((target * 5).convert(1_deg), 100);
    /*if (this->target == target) {
        return;
    } 
    this->target = target;
    this->almpc->forceRemovePath("_");
    const okapi::RQuantity ratio = 1_in / 1_deg; // Transform angle into length
    okapi::QAngle curPos = this->motor->getPosition() * 1_deg;
    this->almpc->generatePath({curPos * ratio, target * ratio}, "_");
    this->almpc->setTarget("_", target < curPos);
    this->almpc->flipDisable(false);*/
}
void Arm::movePower(float power) {
    this->motor->moveVelocity(power * 100);
}

void Arm::waitUntilSettled() {
    this->almpc->waitUntilSettled();
}

void Arm::loop() {
};