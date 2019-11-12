#include "main.h"
#include<bits/stdc++.h> 
#include<string>

using namespace std;

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

pros::Motor backleft_mtr(1);
pros::Motor backright_mtr(2);
pros::Motor frontleft_mtr(3);
pros::Motor frontright_mtr(4);
pros::Motor drive[4]
{
	backleft_mtr,
	backright_mtr,
	frontleft_mtr,
	frontright_mtr
};
pros::Motor angler(5);
pros::Motor lift(6);
pros::Motor left_intake(7);
pros::Motor right_intake(8);

//AUTON FUNCTIONS
double pid(double err, double prevErr) //don't use this function directly just use move()
{
	//PID stuff
	double integral = 0, deriv, pid;
	double ce, ci, cd; //coefficients need to be determined through testing actual bot
	//error pushes it, integral needs to be small (gives it last push), deriv slows it as it reaches target
	//360 encoder units = 1 rotation of motor

	integral += err;
	deriv = err - prevErr;
	pid = (ce * err) + (ci * integral) + (cd * deriv);

	return pid;
}

void move(int destination)
{
	double prevError = 0;
	double error;
	for(pros::Motor x : drive)
	{
		x.tare_position();
	}

	if(destination > 0)
	{
		while(frontright_mtr.get_position() < destination && frontleft_mtr.get_position() < destination && backright_mtr.get_position() < destination && backleft_mtr.get_position() < destination)
		{
			for(pros::Motor x : drive)
			{
				error = destination - x.get_position();
				x = pid(error, prevError);
				prevError = error;
			}
			pros::delay(10);
		}
	}

	else if(destination < 0)
	{
		while(frontright_mtr.get_position() > destination && frontleft_mtr.get_position() > destination && backright_mtr.get_position() > destination && backleft_mtr.get_position() > destination)
		{
			for(pros::Motor x : drive)
			{
				error = -destination + x.get_position();
				x = -pid(error, prevError);
				prevError = error;
			}
			pros::delay(10);
		}
	}	
}

void rightTurn(int turn)
{
	frontright_mtr.move_relative(-turn, -75);
	backright_mtr.move_relative(-turn, -75);
	frontleft_mtr.move_relative(turn, 75);
	backright_mtr.move_relative(turn, 75);
}

void leftTurn(int turn)
{
	frontright_mtr.move_relative(turn, 75);
	backright_mtr.move_relative(turn, 75);
	frontleft_mtr.move_relative(-turn, -75);
	backright_mtr.move_relative(-turn, -75);
}

void anglerShift(int set) //0 for intaking, 1 for stacking
{
	if(set == 0)
	{
		angler.move_absolute(0, 100);
	}
	else if(set == 1)
	{
		angler.move_absolute(600, 100);
	}
}
	


bool toggle = false;

//DRIVER CONTROL
void opcontrol() {


	while (true) {
		
	int left = master.get_analog(ANALOG_LEFT_Y);
	int right = master.get_analog(ANALOG_RIGHT_Y);
	

	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
	{
		if(!toggle)
		{
			toggle = true;
		}
		else
		{
			toggle = false;
		}
	}

	if (!toggle)
	{
		//DRIVE
		//LEFT
		if (left > 20 && left < 70)
		{
			backleft_mtr = left;
			frontleft_mtr = left;
		}
		else if (left < -20 && left > -70)
		{
			backleft_mtr = left;
			frontleft_mtr = left;
		}
		else if (left > 70)
		{
			backleft_mtr = 70;
			frontleft_mtr = 70;
		}
		else if (left < -70)
		{
			backleft_mtr = -70;
			frontleft_mtr = -70;
		}
		else
		{
			backleft_mtr = 0;
			frontleft_mtr = 0;
			frontleft_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			backleft_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
		}
		
		//RIGHT
		if (right > 20 && right < 70)
		{
			backright_mtr = -right;
			frontright_mtr = -right;
		}
		else if (right < -20 && right > -70)
		{
			backright_mtr = -right;
			frontright_mtr = -right;
		}
		else if (right > 70)
		{
			backright_mtr = -70;
			frontright_mtr = -70;
		}
		else if (right < -70)
		{
			backright_mtr = 70;
			frontright_mtr = 70;
		}
		else
		{
			backright_mtr = 0;
			frontright_mtr = 0;
			frontright_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			backright_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
		}
		
		//LIFT
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			lift = -75;
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
		{
			lift = 100;
		}
		else
		{
			lift = 1; //gives motor so little power that it can't actually move the lift
			lift.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE); //doesn't work?
		}

		//ANGLER
		//need to set two positions of angler
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		{
			angler = 50;
			//angler.move_absolute(600, 100);
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		{
			angler = -70;
			//angler.move_absolute(0, -100);
		}

		//INTAKE
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
		{
			left_intake = -70;
			right_intake = 70;
		}
		else
		{
			left_intake = 0;
			right_intake = 0;
		}
		pros::delay(20);
	}

	else if (toggle)
	{
		angler = right * 0.5;
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
		{
			left_intake = -70;
			right_intake = 70;
		}
		else
		{
			left_intake = 0;
			right_intake = 0;
		}
		pros::delay(20);
	}
	
}
}
