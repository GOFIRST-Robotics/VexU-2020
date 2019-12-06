#pragma once

#include "system.h"

// Theoretical top speed is ~1 m/s
#define DRIVE_MP_SPEED 0.5 // m/s
#define DRIVE_MP_ACC 0.5 // m/s^2
#define DRIVE_MP_JERK 10 // m/s^3

class TankDrive: public System {
    public:
    TankDrive(std::shared_ptr<okapi::MotorGroup> leftDT, std::shared_ptr<okapi::MotorGroup> rightDT, okapi::QLength wheelDia, okapi::QLength wheelbase);
    ~TankDrive();
    std::shared_ptr<okapi::ChassisController> getChassis();

    void preparePath(std::initializer_list<okapi::PathfinderPoint> iwaypoints, const std::string &ipathId);
    void drivePath(const std::string &pathid);
    void drivePathReverse(const std::string &pathid);
    void waitUntilSettled();
    void pointToAngle(okapi::QAngle angle);

    enum State {
        OpenLoop = 0,
        Pathfinder,
        Driving,
        Turning
    };

    private:
    std::shared_ptr<okapi::ChassisController> chassis;
    std::shared_ptr<okapi::AsyncMotionProfileController> mpController;
    TankDrive::State state = TankDrive::State::OpenLoop;
    void loop() override;
};
