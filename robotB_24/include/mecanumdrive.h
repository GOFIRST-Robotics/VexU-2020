#pragma once

#include "gopher/system.h"

// Theoretical top speed is ~1 m/s
#define DRIVE_MP_SPEED 0.5 // m/s
#define DRIVE_MP_ACC 0.5 // m/s^2
#define DRIVE_MP_JERK 10 // m/s^3

class MecanumDrive: public System {
    public:
    MecanumDrive(std::shared_ptr<okapi::AbstractMotor> fl, std::shared_ptr<okapi::AbstractMotor> fr, 
                 std::shared_ptr<okapi::AbstractMotor> rl, std::shared_ptr<okapi::AbstractMotor> rr, okapi::QLength wheelDia, okapi::QLength wheelbase);
    std::shared_ptr<okapi::ChassisController> getChassis();

    void preparePath(std::initializer_list<okapi::PathfinderPoint> iwaypoints, const std::string &ipathId);
    void drivePath(const std::string &pathid);
    void drivePathReverse(const std::string &pathid);
    void waitUntilSettled();
    void pointToAngle(okapi::QAngle angle);
    void drive(float x, float y, float rot);

    enum State {
        OpenLoop = 0,
        Pathfinder,
        Driving,
        Turning
    };

    private:
    std::shared_ptr<okapi::ChassisController> chassis;
    std::shared_ptr<okapi::AsyncMotionProfileController> mpController;
    MecanumDrive::State state = MecanumDrive::State::OpenLoop;
    void loop() override;
};
