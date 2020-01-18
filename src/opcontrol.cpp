#include "Robot.hpp"
#include "opcontrol.hpp"
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

pros::Controller master(pros::E_CONTROLLER_MASTER);

void opcontrol() {
	
	while (true) {
		pros::lcd::set_text(5, "backleft: " + std::to_string(sRobot->getMotor("BackLeft")->get_position()));
		pros::lcd::set_text(6, "lift + " + std::to_string(sRobot->getAnalogSensor("Lift Potentiometer")->get_value()));
		
		//Drivetrain

		int left = /*127*std::pow((*/master.get_analog(ANALOG_LEFT_Y)/*/127), (11/7))*0.9*/;
		int right = /*127*std::pow((*/master.get_analog(ANALOG_RIGHT_X)/*/127), (11/7))*0.9*/;

		if(master.get_digital(DIGITAL_A)) {
			sRobot->moveVel(-70);
			sRobot->intakeOut();
		} else {
			sRobot->arcade(left,right);
		}

		// INTAKE
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			sRobot->intakeIn();
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			sRobot->intakeOut();
		} else {
			sRobot->intakeStop();
		}

		// TODO Angler / Lift Macro + Manual

		pros::delay(20);
	}
}
