#include "main.h"
#include "gopher/drivetrain.h"

using namespace okapi::literals;

std::shared_ptr<TankDrive> drive;
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	drive = std::make_shared<TankDrive>(std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(1), okapi::Motor(11), okapi::Motor(-12)})), 
										   std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(-10), okapi::Motor(19), okapi::Motor(-20)})), 
										   3.5_in, 12_in);
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
	auto lift = okapi::MotorGroup({okapi::Motor(9), okapi::Motor(2)});
	lift.setGearing(okapi::AbstractMotor::gearset::red);
	auto claw = okapi::MotorGroup({okapi::Motor(5), okapi::Motor(6)});
	claw.setGearing(okapi::AbstractMotor::gearset::red);

	while (true) {
		int x = master.get_analog(ANALOG_LEFT_Y);
		int rot = master.get_analog(ANALOG_LEFT_X);
		drive->getChassis()->model().arcade(x / 127.0, rot / 127.0);
		bool liftUp = master.get_digital(DIGITAL_R2);
		bool liftDown = master.get_digital(DIGITAL_R1);
		int liftspd = liftUp ? 100 : (liftDown ? -100 : 0);
		bool clawIn = master.get_digital(DIGITAL_L1);
		bool clawOut = master.get_digital(DIGITAL_L2);
		int clawspd = clawIn ? 100 : (clawOut ? -100 : 0);
		lift.moveVelocity(liftspd);
		claw.moveVelocity(clawspd);
		pros::delay(20);
	}
}
