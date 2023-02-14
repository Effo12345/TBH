#include "main.h"
#include "xlib/flywheel.hpp"

using namespace xlib;

//Example flywheel declaration with selector dependency
Selector sel {{}};
Flywheel fw(1, 0.00025f, sel);

//Dummy controller
Controller master;

//Flywheel velocity, stored as pair
//The first value is the RPM, the second value is the TBH control variable
std::pair<int, float> fwVel {2425, 0.866};

//To find the TBH value, find the voltage that spins the flywheel at the desired
//RPM. Normalize it by dividing by 12000; for this example, 11196 mV / 12000 = 
//0.933. Plug this value into the following equation (based on the TBH code):
	//targetDrive = 0.5(initialDrive + TBH)
//This is run whenever the measured velocity passes the target. If the flywheel
//is spinning up, assume that the initial drive value is 1
	//0.933 = 0.5(1 + TBH)
	//1.866 = 1 + TBH
	//TBH = 0.866


void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void opcontrol() {
	while(true) {
		//Set flywheel velocity using moveVelocity
		if(master[ControllerDigital::R1].changedToPressed())
			fw.moveVelocity(fwVel.first, fwVel.second);

		pros::delay(20);
	}
}
