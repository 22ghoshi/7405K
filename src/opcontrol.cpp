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
			sRobot->towerSet(true);
			sRobot->anglerSet(1800);
			pros::delay(500);
			sRobot->liftSet(1053);
			pros::delay(1500);
			sRobot->towerSet(false);
		}

		if (sRobot->getMaster()->get_digital(DIGITAL_UP)) {
			sRobot->towerSet(true);
			sRobot->anglerSet(200);
			pros::delay(500);
			sRobot->liftSet(1520);
			pros::delay(1500);
			sRobot->towerSet(false);
		}

		pros::delay(20);
	}
}
