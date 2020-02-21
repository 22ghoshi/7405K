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
	while (true) { //TODO TOWER MACROS
		
		if (sRobot->getMaster()->get_digital(DIGITAL_RIGHT)) {
			sRobot->amoveSet(true);
			sRobot->anglerSet(2050);
			pros::delay(500);
			sRobot->lmoveSet(true);
			sRobot->liftSet(800);
			pros::delay(1500);
			sRobot->lmoveSet(false);
			sRobot->amoveSet(false);
		}

		if (sRobot->getMaster()->get_digital(DIGITAL_UP)) {
			sRobot->amoveSet(true);
			sRobot->anglerSet(2100);
			pros::delay(500);
			sRobot->lmoveSet(true);
			sRobot->liftSet(1265);
			pros::delay(1500);
			sRobot->lmoveSet(false);
			sRobot->amoveSet(false);
		}

		if (sRobot->getMaster()->get_digital(DIGITAL_DOWN)) {
			sRobot->lmoveSet(true);
			sRobot->liftSet(100);
			while(sRobot->getAnalogSensor("Lift Potentiometer")->get_value() > 100) {
				pros::delay(2);
			}
			sRobot->lmoveSet(false);
			sRobot->amoveSet(true);
			sRobot->anglerSet(410);
			while (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() > 420) {
				pros::delay(2);
			}
			sRobot->amoveSet(false);
		}

		pros::delay(20);
	}
}
