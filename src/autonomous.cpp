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

void topRed() {
	
}

void bottomRed() {  // TODO bottomRed auton first
	//deploy
    sRobot->intakeOut(-127);
	pros::delay(750);

    //intake 4 row
	sRobot->intakeIn(127);
	sRobot->moveDist(2200, 35);
	sRobot->intakeStop();

    //turn and drive to scoring zone
	sRobot->turn(146);
	sRobot->moveDist(1900, 60);
	sRobot->moveDist(-200, -127);

    //stack
	sRobot->amoveSet(true);
	sRobot->anglerSet(3550);
	sRobot->intakeOut(-50);
	pros::delay(500);
	sRobot->intakeIn(50);
	pros::delay(500);
	sRobot->intakeOut(-60);
	pros::delay(600);
	sRobot->amoveSet(false);
	sRobot->intakeStop();
	pros::delay(100);

    //back out from stack
	sRobot->intakeOut(-100);
	*(sRobot->getMotor("Lift")) = -40;
	sRobot->moveVel(-65);
	pros::delay(500);
	sRobot->moveVel(0);
	*(sRobot->getMotor("Lift")) = 0;
	sRobot->intakeStop();
}

void topBlue() {

}

void bottomBlue() {
	//deploy
    sRobot->intakeOut(-127);
	pros::delay(750);

    //intake 4 row
	sRobot->intakeIn(127);
	sRobot->moveDist(2200, 35);
	sRobot->intakeStop();

	//intake cube next to tower
	// sRobot->turn(25);
	// sRobot->moveDist(700, 35);
	// pros::delay(250);
	// sRobot->moveDist(-700, -35);
	// sRobot->turn(-25);
	// sRobot->intakeStop();

    //turn and drive to scoring zone
	sRobot->turn(-147.5);
	sRobot->moveDist(1900, 60);
	sRobot->moveDist(-200, -127);

    //stack
	sRobot->amoveSet(true);
	sRobot->anglerSet(3550);
	sRobot->intakeOut(-50);
	pros::delay(500);
	sRobot->intakeIn(50);
	pros::delay(500);
	sRobot->intakeOut(-60);
	pros::delay(600);
	sRobot->amoveSet(false);
	sRobot->intakeStop();
	pros::delay(100);

    //back out from stack
	sRobot->intakeOut(-100);
	*(sRobot->getMotor("Lift")) = -40;
	sRobot->moveVel(-65);
	pros::delay(500);
	sRobot->moveVel(0);
	*(sRobot->getMotor("Lift")) = 0;
	sRobot->intakeStop();
}

void test() {  //TODO turn/strafe/diag pid testing here
    sRobot->strafe(500);
	sRobot->strafe(-500);
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
	sRobot->anglerSet(1400);
	sRobot->amoveSet(true);
	pros::delay(750);
	sRobot->anglerSet(590);
	pros::delay(750);
	sRobot->amoveSet(false);
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