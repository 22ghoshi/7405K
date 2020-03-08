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

void bottomRed() {
	//deploy
    sRobot->intake(-127);
	pros::delay(1000);

    //intake 4 row
	sRobot->intake(127);
	sRobot->moveDist(2200, 30);
	sRobot->intakeStop();

    //back up, turn, go to scoring zone
	sRobot->turn(-147.5);
	sRobot->moveDist(1500, 90);

    // //stack
	// sRobot->amoveSet(true);
	// sRobot->anglerSet(2500);
	// sRobot->intake(-50);
	// pros::delay(600);
	// sRobot->intake(50);
	// pros::delay(300);
	// sRobot->intake(-60);
	// pros::delay(600);
	// sRobot->amoveSet(false);
	// sRobot->intakeStop();
	// pros::delay(100);

    // //back out from stack
	// sRobot->intake(-80);
	// *(sRobot->getMotor("Lift")) = -40;
	// sRobot->moveVel(-100);
	// pros::delay(500);
	// sRobot->moveVel(0);
	// *(sRobot->getMotor("Lift")) = 0;
	// sRobot->intakeStop();
}

void topBlue() {
	//deploy
	sRobot->moveDist(500, 127);
	sRobot->intake(-127);
	sRobot->moveDist(-500, -127);

	//intake preload + first cube
	sRobot->intake(127);
	sRobot->moveDist(1400, 50);

	// //turn and get second cube
	sRobot->turn(-50);
	sRobot->moveDist(400, 90);

	sRobot->turn(-50);
	sRobot->moveDist(700, 127);



	//stack
	sRobot->amoveSet(true);
	sRobot->anglerSet(2500);
	sRobot->intake(-50);
	pros::delay(600);
	sRobot->intake(50);
	pros::delay(300);
	sRobot->intake(-60);
	pros::delay(600);
	sRobot->amoveSet(false);
	sRobot->intakeStop();
	pros::delay(100);

    //back out from stack
	sRobot->intake(-100);
	*(sRobot->getMotor("Lift")) = -40;
	sRobot->moveVel(-65);
	pros::delay(500);
	sRobot->moveVel(0);
	*(sRobot->getMotor("Lift")) = 0;
	sRobot->intakeStop();
	
}

void bottomBlue() {
	//deploy
    sRobot->intake(-127);
	pros::delay(750);

	sRobot->intake(127);
	sRobot->moveDist(2200, 35);
	sRobot->intakeStop();

    //turn and drive to scoring zone
	sRobot->turn(147.5);
	sRobot->moveDist(1500, 90);
	//sRobot->moveDist(-200, -127);

    //stack
	// sRobot->amoveSet(true);
	// sRobot->anglerSet(3550);
	// sRobot->intake(-50);
	// pros::delay(500);
	// sRobot->intake(50);
	// pros::delay(500);
	// sRobot->intake(-60);
	// pros::delay(600);
	// sRobot->amoveSet(false);
	// sRobot->intakeStop();
	// pros::delay(100);

    //back out from stack
	// sRobot->intake(-100);
	// *(sRobot->getMotor("Lift")) = -40;
	// sRobot->moveVel(-65);
	// pros::delay(500);
	// sRobot->moveVel(0);
	// *(sRobot->getMotor("Lift")) = 0;
	// sRobot->intakeStop();
}

void test() {  //TODO forward/turn/strafe/diag pid testing here
	double offset = (sRobot->getMotor("BackLeft")->get_position() + sRobot->getMotor("BackRight")->get_position()) / 2;
	pros::lcd::set_text(2, "turning");
	sRobot->turn(150);
	double pos = ((sRobot->getMotor("BackLeft")->get_position() + sRobot->getMotor("BackRight")->get_position()) / 2) - offset;
	pros::lcd::set_text(3, std::to_string(pos));
}

void push() {
	sRobot->moveDist(-1550, -127);
	sRobot->intake(-127);
	sRobot->moveDist(1550, 127);
	sRobot->intake(0);
	sRobot->amoveSet(true);
	sRobot->anglerSet(1500);
	pros::delay(1000);
	sRobot->anglerSet(400);
	pros::delay(500);
	sRobot->amoveSet(false);
}

void autonomous() {
	sRobot->startTask("Angler PID", Robot::anglerPID);
	sRobot->startTask("Lift PID", Robot::liftPID);
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