#include "main.h"
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

pros::Motor backleft_mtr(6);
pros::Motor backright_mtr(4);
pros::Motor frontleft_mtr(5);
pros::Motor frontright_mtr(7);
pros::Motor drive[4]
{
	backleft_mtr,
	backright_mtr,
	frontleft_mtr,
	frontright_mtr
};
pros::Motor angler(9);
pros::Motor lift(8);
pros::Motor left_intake(1);
pros::Motor right_intake(2);

//AUTON FUNCTIONS
double integral = 0;
double pid(double err, double prevErr) //don't use this function directly just use move()
{
	//PID stuff
	double deriv, pid;
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
	integral = 0;
	for(pros::Motor x : drive)
	{
		x.tare_position();
	}

	if(destination > 0)
	{
		prevError = destination;
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
		prevError = -destination;
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
		angler.move_absolute(0, 80);
	}
	else if(set == 1)
	{
		angler.move_absolute(600, 80);
	}
}

void intake(int set)
{
	if(set == 1)
	{
		left_intake = -70;
		right_intake = 70;
	}
	else if(set == 0)
	{
		left_intake = 0;
		right_intake = 0;
	}
}

/*void brake_hold(pros::Motor motor)
{
	double pos = motor.get_position();
	int pow = 0;
	pros::lcd::set_text(6, to_string(pos));
	pros::delay(100);
	while(motor.get_position() != pos && motor.get_position() < pos && pow < 50)
	{
		pros::lcd::set_text(5, "Motor pos = " + to_string(motor.get_position()));
		pros::lcd::set_text(6, "Pos = " + to_string(pos));
		pos = motor.get_position();
		pow = pow + 1;
		pros::lcd::set_text(7, to_string(pow));
		motor = pow;
		pros::delay(100);
	}
}*/
	


int toggle = 0;
bool xPressed = false;
bool anglerbrake = false;

//DRIVER CONTROL
void opcontrol() {


	while (true) {
		
	int left = -1 * master.get_analog(ANALOG_RIGHT_X);
	int right = master.get_analog(ANALOG_LEFT_Y);
	//controller dampening
	int left1 = (int) std::round(127.0 * std::pow((left1 / 127), (11 / 7)));
	int right1 = (int) std::round(127.0 * std::pow((right1 / 127), (11 / 7)));
	int final_left, final_right, turn;
	int x = master.get_digital(DIGITAL_X);
	
	//toggle between drive / angler mode
	if(x == 1 && !xPressed)
	{
		toggle = 1 - toggle;
		xPressed = true;
		pros::lcd::set_text(4, to_string(toggle));
	}
	else if(x == 0)
	{
		xPressed = false;
	}

	
	

	//normal drive mode, normal controls
	if (toggle == 0)
	{
		pros::lcd::set_text(4, "DRIVE");
		//DRIVE
		//TANK DRIVE
		/*if((left1 < -20 && left1 > -100) || (left1 > 20 && left1 < 100))
		{
			backleft_mtr = left;
			frontleft_mtr = left;
			
		}
		else
		{
			backleft_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			frontleft_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			backleft_mtr = 0;
			backright_mtr = 0;	
		}

		if((right1 < -20 && right1 > -100) || (right1 > 20 && right1 < 100))
		{
			backright_mtr = -right;
			frontright_mtr = -right;
		}
		else
		{
			backright_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			frontright_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			backright_mtr = 0;
			frontright_mtr = 0;
		}*/

		/*backright_mtr = -right1;
		backleft_mtr = left1;
		frontright_mtr = -right1;
		frontleft_mtr = left1;*/

		//ARCADE DRIVE
		backleft_mtr = (left - right);
		backright_mtr = (left + right);
		frontleft_mtr = (left - right);
		frontright_mtr =  (left + right);
	pros::lcd::set_text(5, to_string(left1));
	pros::lcd::set_text(6, to_string(right1));


		//LIFT
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			lift = -75;
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
		{
			lift = 75;
		}
		else
		{
			lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); //doesn't work?
			lift = 0; //gives motor so little power that it can't actually move the lift
		}

		//ANGLER
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		{
			angler = 85;
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		{
			angler = -85;
		}
		else
		{
			angler.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			angler = 0;
		}

		//INTAKE
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
		{
			left_intake = -70;
			right_intake = 70;
		}
		else
		{
			left_intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			right_intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			left_intake = 0;
			right_intake = 0;
		}
		pros::delay(20);
	}

	//angler drive mode, angler controlled with left joystick here
	else if (toggle == 1)
	{
		pros::lcd::set_text(4, "ANGLER");
		if(right < 20 && right > -20)
		{
			angler.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			angler = 0;
		}
		else if(right > -100 && right < 100)
		{
			angler = right * 0.5;
		}
		
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		{
			left_intake = 35;
			right_intake = -35;
		}
		else
		{
			left_intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			right_intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			left_intake = 0;
			right_intake = 0;
		}
		pros::delay(20);
	}
	
}
}
