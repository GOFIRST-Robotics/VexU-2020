#include "main.h"
#include "mecanumdrive.h"
#include "arm.h"
#include "tray.h"
#include "intake.h"

using namespace okapi::literals;

std::shared_ptr<MecanumDrive> drive;
std::shared_ptr<Tray> tray;
std::shared_ptr<Arm> arm;
std::shared_ptr<Intake> intake;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	drive = std::make_shared<MecanumDrive>(std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(7), okapi::Motor(-8)})), 
										   std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(3), okapi::Motor(-4)})), 
										   std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(-9), okapi::Motor(10)})), 
										   std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(-1), okapi::Motor(2)})),
										   6_in, 12_in);
	tray = std::make_shared<Tray>(std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(14)})));
	arm = std::make_shared<Arm>(std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(-15), okapi::Motor(16)})));
	intake = std::make_shared<Intake>(std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(-11), okapi::Motor(-12)})),
									  std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(19), okapi::Motor(20)})));
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	while (true) {
		int x = master.get_analog(ANALOG_LEFT_X);
		int y = master.get_analog(ANALOG_LEFT_Y);
		int rot = master.get_analog(ANALOG_RIGHT_X);

		if (master.get_digital(DIGITAL_A)) {
			intake->intake();
		}
		else if (master.get_digital(DIGITAL_B)) {
			intake->reverse();
		}
		else {
			intake->idle();
		}
		if (master.get_digital(DIGITAL_R1)) {
			arm->movePower(1.0);
		}
		else if (master.get_digital(DIGITAL_R2)) {
			arm->movePower(-1.0);
		}
		else {
			arm->movePower(0.0);
		}

		if (master.get_digital(DIGITAL_L1)) {
			tray->setAngle(60_deg);
		}
		if (master.get_digital(DIGITAL_L2)) {
			tray->setAngle(0_deg);
		}

		drive->drive(x / 127.0, y / 127.0, rot / 127.0);
		pros::delay(20);
	}
}
