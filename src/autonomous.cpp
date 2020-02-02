// #include "autonomous.hpp"
#include "Robot.hpp"
#include "initialize.hpp"
#include "main.h"

// #include <vector>
// #include <string>

// /**
//  * Runs the user autonomous code. This function will be started in its own task
//  * with the default priority and stack size whenever the robot is enabled via
//  * the Field Management System or the VEX Competition Switch in the autonomous
//  * mode. Alternatively, this function may be called in initialize or opcontrol
//  * for non-competition testing purposes.
//  *
//  * If the robot is disabled or communications is lost, the autonomous task
//  * will be stopped. Re-enabling the robot will restart the task, not re-start it
//  * from where it left off.
//  */

void topRed() {}

void bottomRed() {  // TODO bottomRed auton first
	sRobot->intakeOut(-127);
	pros::delay(750);
	sRobot->intakeIn(127);
	sRobot->moveDist(2600, 50);
	sRobot->intakeStop();
	sRobot->turn(145);
	sRobot->moveDist(2300, 90);
	sRobot->moveDist(-200, -127);
	sRobot->stackSet(true);
	sRobot->anglerSet(1200);
	sRobot->intakeOut(-50);
	pros::delay(500);
	sRobot->intakeIn(60);
	pros::delay(500);
	sRobot->intakeOut(-60);
	pros::delay(1000);
	sRobot->stackSet(false);
	sRobot->intakeStop();
	pros::delay(100);
	sRobot->intakeOut(-100);
	*(sRobot->getMotor("Lift")) = -40;
	sRobot->moveVel(-65);
	pros::delay(500);
	sRobot->moveVel(0);
	*(sRobot->getMotor("Lift")) = 0;
	sRobot->intakeStop();
}

void topBlue() {}

void bottomBlue() {
	sRobot->intakeOut(-127);
	pros::delay(750);
	sRobot->intakeIn(127);
	sRobot->moveDist(2600, 50);
	sRobot->intakeStop();
	sRobot->turn(-147);
	sRobot->moveDist(2300, 90);
	sRobot->moveDist(-200, -127);
	sRobot->stackSet(1);
	sRobot->anglerSet(1200);
	sRobot->intakeOut(-50);
	pros::delay(500);
	sRobot->intakeIn(60);
	pros::delay(500);
	sRobot->intakeOut(-60);
	pros::delay(1000);
	sRobot->stackSet(0);
	sRobot->intakeStop();
	pros::delay(100);
	sRobot->intakeOut(-100);
	*(sRobot->getMotor("Lift")) = -40;
	sRobot->moveVel(-65);
	pros::delay(500);
	sRobot->moveVel(0);
	*(sRobot->getMotor("Lift")) = 0;
	sRobot->intakeStop();
}

void test() {  // TODO turn pid testing here
}

void push() {
	sRobot->intakeOut(-127);
	pros::delay(1000);
	sRobot->intakeStop();
	pros::delay(500);
	sRobot->moveVel(-90);
	pros::delay(750);
	sRobot->moveVel(0);
	pros::delay(750);
	sRobot->moveDist(1500, 127);
}

void autonomous() {
	sRobot->startTask("Angler PID", Robot::anglerPID);
	switch (autonselect) {
		case autonSelect::topRed:
			topRed();
			break;
		case autonSelect::botRed:
			bottomRed();
			break;
		case autonSelect::topBlue:
			topBlue();
			break;
		case autonSelect::botBlue:
			bottomBlue();
			break;
		case autonSelect::test:
			test();
			break;
		case autonSelect::push:
			push();
			break;
		default:
			push();
			break;
	}
}