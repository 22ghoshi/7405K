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

pros::Motor backleft_mtr(3);
pros::Motor backright_mtr(1);
pros::Motor frontleft_mtr(2);
pros::Motor frontright_mtr(4);
pros::Motor leftDrive[2]
{
	backleft_mtr,
	frontleft_mtr
};
pros::Motor rightDrive[2]
{
	backright_mtr,
	frontright_mtr
};
pros::Motor angler(7, pros::E_MOTOR_GEARSET_36);
pros::Motor lift(5, pros::E_MOTOR_GEARSET_36);
pros::Motor left_intake(20, pros::E_MOTOR_GEARSET_36);
pros::Motor right_intake(19, pros::E_MOTOR_GEARSET_36);

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
	int res;
	
	backleft_mtr.tare_position();
	frontleft_mtr.tare_position();
	backright_mtr.tare_position();
	frontright_mtr.tare_position();

	if(destination > 0)
	{
		prevError = destination;
		while(backleft_mtr.get_position() < destination)
		{
			error = destination - backleft_mtr.get_position();
			res = pid(error, prevError);
			prevError = error;
			backleft_mtr = res;
			frontleft_mtr = res;
			backright_mtr = -res;
			frontright_mtr = -res;
		}
	}

	if(destination < 0)
	{
		while(backleft_mtr.get_position() > destination)
		{
			error = -1 * (destination - backleft_mtr.get_position());
			res = pid(error, prevError);
			prevError = error;
			backleft_mtr = -res;
			frontleft_mtr = -res;
			backright_mtr = res;
			frontright_mtr = res;
		}
	}
}

void rightTurn(int turn)
{
	backleft_mtr.tare_position();
	backright_mtr.tare_position();
	frontleft_mtr.tare_position();
	frontright_mtr.tare_position();
	frontright_mtr.move_absolute(turn, -75);
	backright_mtr.move_absolute(turn, -75);
	frontleft_mtr.move_absolute(turn, 75);
	backright_mtr.move_absolute(turn, 75);
}

void leftTurn(int turn)
{
	backleft_mtr.tare_position();
	backright_mtr.tare_position();
	frontleft_mtr.tare_position();
	frontright_mtr.tare_position();
	frontright_mtr.move_absolute(-turn, 75);
	backright_mtr.move_absolute(-turn, 75);
	frontleft_mtr.move_absolute(-turn, -75);
	backright_mtr.move_absolute(-turn, -75);
}

void driveMove(int dist)
{
	backleft_mtr.tare_position();
	backright_mtr.tare_position();
	frontleft_mtr.tare_position();
	frontright_mtr.tare_position();
	backleft_mtr.move_absolute(dist, 90);
	backright_mtr.move_absolute(-dist, 90);
	frontleft_mtr.move_absolute(dist, 90);
	frontright_mtr.move_absolute(-dist, 90);
	if(dist < 0)
	{
		while(backleft_mtr.get_position() > (dist + 10))
		{
			pros::delay(2);
		}
	}
	if(dist > 0)
	{
		while(backleft_mtr.get_position() < (dist - 10))
		{
			pros::delay(2);
		}
	}
	pros::delay(250);
}

void driveMove(int dist, int speed)
{
	backleft_mtr.tare_position();
	backright_mtr.tare_position();
	frontleft_mtr.tare_position();
	frontright_mtr.tare_position();
	backleft_mtr.move_absolute(dist, speed);
	backright_mtr.move_absolute(-dist, speed);
	frontleft_mtr.move_absolute(dist, speed);
	frontright_mtr.move_absolute(-dist, speed);
	if(dist < 0)
	{
		while(backleft_mtr.get_position() > (dist + 10))
		{
			pros::delay(2);
		}
	}
	if(dist > 0)
	{
		while(backleft_mtr.get_position() < (dist - 10))
		{
			pros::delay(2);
		}
	}
	pros::delay(250);
}

void intake(int set)
{
    if(set == 1)
    {
        left_intake = 127;
        right_intake = -127; 
    }
    else if(set == 0)
    {
        left_intake = 0;
        right_intake = 0; 
    }
    else if(set == -1)
    {
        left_intake = -40;
        right_intake = 40;
    }
}

void anglerMove(int dist)
{
	angler.move_absolute(dist, 70);
	if(dist < 0)
	{
		while(angler.get_position() > (dist + 10))
		{
			pros::delay(2);
		}
	}
	if(dist > 0)
	{
		while(angler.get_position() < (dist - 10))
		{
			pros::delay(2);
		}
	}
	pros::delay(250);
}

void backout(int time)
{
	backleft_mtr = -55;
	backright_mtr = 55;
	frontleft_mtr = -55;
	frontright_mtr = 55;
    left_intake = -100;
    right_intake = 100;
    pros::delay(time);
    backleft_mtr = 0;
	backright_mtr = 0;
	frontleft_mtr = 0;
	frontright_mtr = 0;
    left_intake = 0;
    right_intake = 0;
}

//DRIVER CONTROL
int left1;
int right1;
void opcontrol() {


	while (true) {
		
	int left = master.get_analog(ANALOG_LEFT_Y);
	int right = master.get_analog(ANALOG_RIGHT_X);
	//controller dampening
	if(left < -10)
	{
		left1 = -127.0 * std::pow((double) (-1 * left) / 127, (double) 11 / 7);
	}
	else if(left > 10)
	{
		left1 = 127.0 * std::pow((double) left / 127, (double) 11 / 7);
	}
	else
	{
		left1 = 0;
	}

	if(right < -10)
	{
		right1 = -127.0 * std::pow((double) (-1 * right) / 127, (double) 11 / 7);
	}
	else if(right > 10)
	{
		right1 = 127.0 * std::pow((double) right / 127, (double) 11 / 7);
	}
	else
	{
		right1 = 0;
	}
	int x = master.get_digital(DIGITAL_X);
	int a  = master.get_digital(DIGITAL_A);
	int up = master.get_digital(DIGITAL_UP);
	int down = master.get_digital(DIGITAL_DOWN);
	int leftb = master.get_digital(DIGITAL_LEFT);
	int rightb = master.get_digital(DIGITAL_RIGHT);

	
	

	//normal drive mode, normal controls
	
		//DRIVE
		//ARCADE DRIVE
		if(a == 0)
		{
			backleft_mtr = (0.9) * (left1 + right1);
			backright_mtr = (-0.9) * (left1 - right1);
			frontleft_mtr = (0.9) * (left1 + right1);
			frontright_mtr =  (-0.9) * (left1 - right1);
		}
		else if(a == 1)
		{
			backleft_mtr = -55;
			backright_mtr = 55;
			frontleft_mtr = -55;
			frontright_mtr = 55;
		}
			
		pros::lcd::set_text(5, std::to_string(angler.get_position()));
		pros::lcd::set_text(6, std::to_string(angler.get_position()));

		//LIFT
		if(master.get_digital(DIGITAL_B))
		{
			lift = -100;
		}
		else if(master.get_digital(DIGITAL_Y))
		{
			lift = 127;
		}
		else
		{
			lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); //doesn't work?
			lift = 0;
		}

		//ANGLER
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		{
			angler = 95;
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		{
			angler = -127;
		}
		else
		{
			angler.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			angler = 0;
		}

		//INTAKE
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
		{
			left_intake = 127;
			right_intake = -127;
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			left_intake = -40;
			right_intake = 40;
		}
		else if(a == 1)
		{
			left_intake = -100;
			right_intake = 100;
		}
		else
		{
			left_intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			right_intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			left_intake = 0;
			right_intake = 0;
		}

		//testing auton on x
		if(x == 1)
		{
			autonomous();
		}

		if(down == 1)
		{
			while(lift.get_position() > 0)
			{
				lift = -100;
				pros::delay(2);
			}
			while(angler.get_position() > 0)
			{
				angler = -100;
				pros::delay(2);
			}
		}
		if(rightb == 1)
		{
			lift.move_absolute(3500, 127);
			angler.move_absolute(1200, 100);
			while(lift.get_position() < 3490 || angler.get_position() < 1190)
			{
				pros::delay(2);
			}
		}

		
		pros::delay(20);
}
}
