#include "main.h"
#include "globals.hpp"

bool fwToggle = false;
int fwVelocity = 2;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
	//Initialize PROS LCD
	pros::lcd::initialize();
	//grapher.initGraph();
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
void autonomous() {	
}

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
	while(true) {

		//Interface values with standard output
		if(master[ControllerDigital::left].changedToPressed())
			interface.update();

		//Calculate flywheel velocity
		//This is placed before the toggle so that the controlVelocity() function
		//does not set the velocity to a value after it has already been set to
		//0 and the check for fwToggle is not performed.
		if(fwToggle)
			fw.controlVelocity();

		//Start and stop the flywheel with a toggle
		if(master[ControllerDigital::A].changedToPressed()) {
			fwToggle = !fwToggle;
			if(fwToggle)
				fw.setVelocity(fwVelocity);
			else
				flyWheel.moveVoltage(0);
		}

		//Incriment flywheel velocity by 2 based on the B button
		if(master[ControllerDigital::B].changedToPressed()) {
			fwVelocity += 2;
			fw.setVelocity(fwVelocity);
		}
			

		pros::delay(20);
	}
}
