#include "opcontrol.hpp"

#include "Robot.hpp"
#include "main.h"


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
	sRobot->startTask("Drive", Robot::drive);
	sRobot->startTask("Lift PID", Robot::liftPID);
	sRobot->startTask("Angler PID", Robot::anglerPID);
	while (true) { //TODO TOWER/DOWN MACROS
		
		if (sRobot->getMaster()->get_digital(DIGITAL_RIGHT)) {
			if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 1200) {
				sRobot->amoveSet(true);
				sRobot->anglerSet(1200);
				while (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 800) {
					pros::delay(2);
				}
			}
			sRobot->lmoveSet(true);
			sRobot->liftSet(2050);
			pros::delay(1500);
			sRobot->lmoveSet(false);
			sRobot->amoveSet(false);
		}

		if (sRobot->getMaster()->get_digital(DIGITAL_UP)) {
			if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 1200) {
				sRobot->amoveSet(true);
				sRobot->anglerSet(1200);
				while (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 800) {
					pros::delay(2);
				}
			}
			sRobot->lmoveSet(true);
			sRobot->liftSet(2450);
			pros::delay(1500);
			sRobot->lmoveSet(false);
			sRobot->amoveSet(false);
		}

		if (sRobot->getMaster()->get_digital(DIGITAL_DOWN)) {
			sRobot->lmoveSet(true);
			sRobot->liftSet(100);
			pros::delay(500);	
			sRobot->amoveSet(true);
			sRobot->anglerSet(430);
			pros::delay(1000);
			sRobot->lmoveSet(false);
			sRobot->amoveSet(false);
		}

		if (sRobot->getMaster()->get_digital(DIGITAL_X)) {
			

			
			// autonomous();
			// break;
		}

		pros::delay(20);
	}
}
