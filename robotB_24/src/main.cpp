#include "main.h"

using namespace okapi::literals;

std::shared_ptr<MecanumDrive> drive;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	drive = std::make_shared<MecanumDrive>(std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(0), okapi::Motor(1)})), 
										   std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(2), okapi::Motor(3)})), 
										   std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(4), okapi::Motor(5)})), 
										   std::make_shared<okapi::MotorGroup>(std::initializer_list<okapi::Motor>({okapi::Motor(5), okapi::Motor(7)})),
										   6_in, 12_in);
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
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int x = master.get_analog(ANALOG_LEFT_Y);
		int y = master.get_analog(ANALOG_LEFT_X);
		int rot = master.get_analog(ANALOG_RIGHT_X);

		drive->drive(x / 127, y / 127, rot / 127);
		pros::delay(20);
	}
}
