#include "mecanumdrive.h"
#include "okapi/api.hpp"
#include "gopher/utils.h"

MecanumDrive::MecanumDrive(std::shared_ptr<okapi::AbstractMotor> fl, std::shared_ptr<okapi::AbstractMotor> fr, 
                 std::shared_ptr<okapi::AbstractMotor> rl, std::shared_ptr<okapi::AbstractMotor> rr, okapi::QLength wheelDia, okapi::QLength wheelbase) 
    : System("MecanumDrive") {
    auto drive_gearset = fr->getGearing();
    this->chassis = okapi::ChassisControllerBuilder()
                    .withMotors(fl, fr, rr, rl)
                    .withDimensions(drive_gearset, {wheelDia, wheelbase})
                    .build();
    
    this->mpController = okapi::AsyncMotionProfileControllerBuilder()
                         .withOutput(*this->chassis)
                         .withLimits(okapi::PathfinderLimits({DRIVE_MP_SPEED, DRIVE_MP_ACC, DRIVE_MP_JERK}))
                         .buildMotionProfileController();
    this->mpController->startThread();
}

void MecanumDrive::drive(float x, float y, float rot) {
    std::static_pointer_cast<okapi::XDriveModel>(this->getChassis()->getModel())->xArcade(x, y, rot);
}

std::shared_ptr<okapi::ChassisController> MecanumDrive::getChassis() {
    return this->chassis;
}

void MecanumDrive::loop() {
    switch(this->state) {
        case MecanumDrive::State::Pathfinder:
        if (this->mpController->isSettled()) {
            this->state = MecanumDrive::State::OpenLoop;
            this->mpController->flipDisable(true);
        }
        break;
        default:
        case MecanumDrive::State::OpenLoop: break;

        
    }
}

void MecanumDrive::preparePath(std::initializer_list<okapi::PathfinderPoint> iwaypoints, const std::string &ipathId) {
    this->mpController->generatePath(iwaypoints, ipathId);
}

void MecanumDrive::drivePath(const std::string &pathid) {
    this->mpController->setTarget(pathid);
    this->mpController->flipDisable(false);
    this->state = MecanumDrive::State::Pathfinder;
}

void MecanumDrive::drivePathReverse(const std::string &pathid) {
    this->mpController->setTarget(pathid, true);
    this->mpController->flipDisable(false);
    this->state = MecanumDrive::State::Pathfinder;
}

void MecanumDrive::waitUntilSettled() {
    while (this->state != MecanumDrive::State::OpenLoop) {
        pros::delay(10);
    }
}

void MecanumDrive::pointToAngle(okapi::QAngle angle) {
    this->getChassis()->turnAngle(angle);
}