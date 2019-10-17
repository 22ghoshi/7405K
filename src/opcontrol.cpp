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
pros::Motor backleft_mtr(4);
pros::Motor backright_mtr(6);
pros::Motor frontleft_mtr(7);
pros::Motor frontright_mtr(5);
pros::Motor angler(8);
pros::Motor lift(9);
pros::Motor left_intake(10);
pros::Motor right_intake(11);

//AUTON FUNCTIONS
double prevErr = 0;
double pid(double err) //to use this, set motor speed to pid(destination - motor.get_position)
{
	//PID stuff
	double integral, deriv, pid;
	double ce, ci, cd; //coefficients need to be determined through testing actual bot
	//error pushes it, integral needs to be small (gives it last push), deriv slows it as it reaches target
	//360 encoder units = 1 rotation of motor

	integral += err;
	deriv = err - prevErr;
	pid = (ce * err) + (ci * integral) + (cd * deriv);
	prevErr = err;

	return pid;
}

void move()
{

}

void rightTurn(int turn)
{
	frontright_mtr.move_relative(-turn, 100);
	backright_mtr.move_relative(-turn, 100);
	frontleft_mtr.move_relative(turn, 100);
	backright_mtr.move_relative(turn, 100);
}

void leftTurn(int turn)
{
	frontright_mtr.move_relative(turn, 100);
	backright_mtr.move_relative(turn, 100);
	frontleft_mtr.move_relative(-turn, 100);
	backright_mtr.move_relative(-turn, 100);
}

void anglerShift(int set) //0 for intaking, 1 for stacking
{
	if(set == 0)
	{
		//angler.move_absolute((short distance), 100)
	}
	else if(set == 1)
	{
		//angler.move_absolute((long distance), 100)
	}
}

void 
	




//DRIVER CONTROL
void opcontrol() {


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
			backright_mtr = 0;
			frontright_mtr = 0;
			frontleft_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			backleft_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		}
		
		if (right > 15 || right < -15)
		{
			backright_mtr = -right;
			frontright_mtr = -right;
		}
		else
		{
			backright_mtr = 0;
			frontright_mtr = 0;
			backright_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			frontright_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		}
		
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			lift = -50;
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
		{
			lift = 100;
		}
		else
		{
			lift = 15; //gives motor so little power that it can't actually move the lift
			lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); //doesn't work?
		}

		//need to set two positions of angler
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
		{
			//angler = 70;
			motor_move_relative(angler, 10, 10);
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
		{
			angler = -70;
		}
		else
		{
			angler = 0;
		}
		

		
		
		
		pros::delay(20);

	}
}
