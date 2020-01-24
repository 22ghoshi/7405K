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

pros::Controller master(pros::E_CONTROLLER_MASTER);
//pros::Motor lift(5);

void opcontrol() {
	while (true) {
		pros::lcd::set_text(5, "Angler: " + std::to_string(sRobot->getAnalogSensor("Angler Potentiometer")->get_value()));
		pros::lcd::set_text(6, "Lift: " + std::to_string(sRobot->getAnalogSensor("Lift Potentiometer")->get_value()));

		// DRIVETRAIN
		int left = /*127*std::pow((*/ master.get_analog(ANALOG_LEFT_Y) /*/127), (11/7))*0.9*/;
		int right = /*127*std::pow((*/ master.get_analog(ANALOG_RIGHT_X) /*/127), (11/7))*0.9*/;

		if (master.get_digital(DIGITAL_A)) {
			sRobot->moveVel(-30);
		} else {
			sRobot->arcade(left, right);
		}

		// INTAKE
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			sRobot->intakeIn();
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) || master.get_digital(DIGITAL_A)) {
			sRobot->intakeOut();
		}
		else {
			sRobot->intakeStop();
		}

		

		// ANGLER
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 2800) {
				*(sRobot->getMotor("Angler")) = 127;
			} else if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 3900) {
				*(sRobot->getMotor("Angler")) = 70;
			}
			else {
				*(sRobot->getMotor("Angler")) = 0;
				sRobot->getMotor("Angler")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			}
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
				*(sRobot->getMotor("Angler")) = -127;
		} else {
			*(sRobot->getMotor("Angler")) = 0;
			sRobot->getMotor("Angler")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		}

		// LIFT
		if (master.get_digital(DIGITAL_Y)) {
			*(sRobot->getMotor("Lift")) = 127;
			master.set_text(0, 0, "lift   up");
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && (sRobot->getAnalogSensor("Lift Potentiometer")->get_value() > 1250)) {
			*(sRobot->getMotor("Lift")) = 70;
		} else if (master.get_digital(DIGITAL_B)) {
			*(sRobot->getMotor("Lift")) = -127;
			master.set_text(0, 0, "lift down");
		} else if (master.get_digital(DIGITAL_A)) {
			*(sRobot->getMotor("Lift")) = -40;
		} else {
			*(sRobot->getMotor("Lift")) = 0;
			sRobot->getMotor("Lift")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		}

		//TODO tower macros
		if (master.get_digital(DIGITAL_UP)) {
			sRobot->tower(1);
		}

		pros::delay(20);
	}
}
