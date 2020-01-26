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
			sRobot->moveVel(-60);
		} else {
			sRobot->arcade(left, right);
		}

		// INTAKE
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			sRobot->intakeIn();
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) || master.get_digital(DIGITAL_A)) {
			sRobot->intakeOut(-127);
		} else {
			sRobot->intakeStop();
		}

		

		// ANGLER
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 2400) {
				*(sRobot->getMotor("Angler")) = 127;
			} else if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 3900) {
				*(sRobot->getMotor("Angler")) = 75;
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
		} else if (master.get_digital(DIGITAL_B)) {
			*(sRobot->getMotor("Lift")) = -127;
			master.set_text(0, 0, "lift down");
		} else if (master.get_digital(DIGITAL_A)) {
			*(sRobot->getMotor("Lift")) = -30;
		} else {
			*(sRobot->getMotor("Lift")) = 0;
			sRobot->getMotor("Lift")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		}

		//TODO tower macros
		if (master.get_digital(DIGITAL_RIGHT)) {
			sRobot->tower(1);
		}
		if (master.get_digital(DIGITAL_UP)) {
			sRobot->tower(2);
		}
		if (master.get_digital(DIGITAL_DOWN)) {
			sRobot->down();
		}

		pros::delay(20);
	}
}
