#include "gopher/drivetrain.h"
#include "okapi/api.hpp"
#include "gopher/utils.h"

TankDrive::TankDrive(std::shared_ptr<okapi::MotorGroup> leftDT, std::shared_ptr<okapi::MotorGroup> rightDT, okapi::QLength wheelDia, okapi::QLength wheelbase) 
    : System("TankDrive") {
    leftDT->setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
    rightDT->setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
    auto drive_gearset = leftDT->getGearing();
    this->chassis = okapi::ChassisControllerBuilder()
                    .withMotors(leftDT, rightDT)
                    .withDimensions(drive_gearset, {wheelDia, wheelbase})
                    .build();
    
    this->mpController = okapi::AsyncMotionProfileControllerBuilder()
                         .withOutput(*this->chassis)
                         .withLimits(okapi::PathfinderLimits({DRIVE_MP_SPEED, DRIVE_MP_ACC, DRIVE_MP_JERK}))
                         .buildMotionProfileController();
    this->mpController->startThread();
}

TankDrive::~TankDrive() {
    
}

std::shared_ptr<okapi::ChassisController> TankDrive::getChassis() {
    return this->chassis;
}

void TankDrive::loop() {
    switch(this->state) {
        case TankDrive::State::Pathfinder:
        if (this->mpController->isSettled()) {
            this->state = TankDrive::State::OpenLoop;
            this->mpController->flipDisable(true);
        }
        break;
        default:
        case TankDrive::State::OpenLoop: break;

        
    }
}

void TankDrive::preparePath(std::initializer_list<okapi::PathfinderPoint> iwaypoints, const std::string &ipathId) {
    this->mpController->generatePath(iwaypoints, ipathId);
}

void TankDrive::drivePath(const std::string &pathid) {
    this->mpController->setTarget(pathid);
    this->mpController->flipDisable(false);
    this->state = TankDrive::State::Pathfinder;
}

void TankDrive::drivePathReverse(const std::string &pathid) {
    this->mpController->setTarget(pathid, true);
    this->mpController->flipDisable(false);
    this->state = TankDrive::State::Pathfinder;
}

void TankDrive::waitUntilSettled() {
    while (this->state != TankDrive::State::OpenLoop) {
        pros::delay(10);
    }
}

void TankDrive::pointToAngle(okapi::QAngle angle) {
    this->getChassis()->turnAngle(angle);
}