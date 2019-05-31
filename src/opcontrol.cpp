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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor backleft_mtr(3);
	pros::Motor backright_mtr(4);
	pros::Motor frontleft_mtr(1);
	pros::Motor frontright_mtr(2);
	pros::Motor lift(5);
	while (true) {
		
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);
		if (left > 15 || left < -15)
		{
			frontleft_mtr = left;
			backleft_mtr = left;
		}
		else
		{
			frontleft_mtr = 0;
			backleft_mtr = 0;
		}
		
		if (right > 15 || right < -15)
		{
			backright_mtr = right;
			frontright_mtr = right;
		}
		else
		{
			backright_mtr = 0;
			frontright_mtr = 0;
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
		{
			lift = -30;
		}
		else
		{
			lift = 0;
		}
		


	}
}
