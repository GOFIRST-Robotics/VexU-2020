#pragma once

#include "system.h"

// Theoretical top speed is ~1 m/s
#define DRIVE_MP_SPEED 0.5 // m/s
#define DRIVE_MP_ACC 0.5 // m/s^2
#define DRIVE_MP_JERK 10 // m/s^3
#define DRIVE_TURN_P 0.6  // %vbus / rad error
#define DRIVE_TURN_I 0.0
#define DRIVE_TURN_D 0.0

class TankDrive: public System, public okapi::ControllerInput<double>, public okapi::ControllerOutput<double> {
    public:
    TankDrive(std::shared_ptr<okapi::AbstractMotor> leftDT, std::shared_ptr<okapi::AbstractMotor> rightDT, okapi::QLength wheelDia, okapi::QLength wheelbase);
    std::shared_ptr<okapi::ChassisController> getChassis();

    void preparePath(std::initializer_list<okapi::PathfinderPoint> iwaypoints, const std::string &ipathId);
    void drivePath(const std::string &pathid);
    void drivePathReverse(const std::string &pathid);
    void waitUntilSettled();
    void pointToAngle(okapi::QAngle angle);
    void pointToPoint(okapi::QLength x, okapi::QLength y);

    double controllerGet() override;
    void controllerSet(double output) override;

    enum State {
        OpenLoop = 0,
        Pathfinder,
        Driving,
        Turning
    };

    private:
    std::shared_ptr<okapi::AbstractMotor> leftMotors;
    std::shared_ptr<okapi::AbstractMotor> rightMotors;
    std::shared_ptr<okapi::ChassisController> chassis;
    std::shared_ptr<okapi::AsyncMotionProfileController> mpController;
    TankDrive::State state = TankDrive::State::OpenLoop;
    void loop() override;
};
