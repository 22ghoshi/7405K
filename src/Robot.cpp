#include "Robot.hpp"

Robot* Robot::pInstance = NULL;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller* Robot::controller =  &master;
pros::Imu inertialSensor(5);
bool Robot::amove = false;
bool Robot::lmove = false;

Robot::Robot() {
	// Starting Values for setpoints
	liftSetPoint.store(5);
	anglerSetPoint.store(11);
	inertialSensor.reset();
	// Motors Init
	motors["BackLeft"] = std::make_unique<pros::Motor>(7, true);
	motors["BackRight"] = std::make_unique<pros::Motor>(9);
	motors["FrontLeft"] = std::make_unique<pros::Motor>(20, true);
	motors["FrontRight"] = std::make_unique<pros::Motor>(10);
	motors["Angler"] = std::make_unique<pros::Motor>(12);
	motors["Lift"] = std::make_unique<pros::Motor>(18, true);
	motors["LeftIntake"] = std::make_unique<pros::Motor>(19, pros::E_MOTOR_GEARSET_36);
	motors["RightIntake"] = std::make_unique<pros::Motor>(13, pros::E_MOTOR_GEARSET_36, true);

	// Sensors Init
	sensorAnalog["Angler Potentiometer"] = std::make_unique<pros::ADIAnalogIn>(1);
	sensorAnalog["Lift Potentiometer"] = std::make_unique<pros::ADIAnalogIn>(2);

	pros::delay(100);
}

Robot* Robot::Instance() {
	if (!pInstance) {
		pInstance = new Robot();
	}
	return pInstance;
}

void Robot::mecanum(int power, int strafe, int turn) {
	*motors["BackLeft"] = power - strafe + turn;
	*motors["BackRight"] = power + strafe - turn;
	*motors["FrontLeft"] = power + strafe + turn;
	*motors["FrontRight"] = power - strafe - turn;
}

void Robot::moveDist(int dist, int limit) { //TODO tune with mecanums
	double time = 0;

	double leftOffset = motors["BackLeft"]->get_position();
	double rightOffset = motors["BackRight"]->get_position();
	double leftPos, rightPos, avgPos = (leftOffset + rightOffset) / 2;
	double prevErr = dist;

	if (dist > 0) { 

		double kP = 0.1175;
		double kI = 0;
		double kD = 0.06;
		double P = 0, I = 0, D = 0;

		while (avgPos < (dist - 5)) {
			if (motors["BackLeft"]->get_actual_velocity() < 2 && time > 500) {
				break;
			}
			leftPos = motors["BackLeft"]->get_position() - leftOffset;
			rightPos = motors["BackRight"]->get_position() - rightOffset;
			avgPos = (leftPos + rightPos) / 2;


			double err = dist - avgPos;
			P = err;
			I += err;
			D = err - prevErr;
			prevErr = err;

			double res = (kP * P) + (kI * I) + (kD * D);
			if ((res < 0 && res < limit) || (res > 0 && res > limit)) {
				*motors["BackLeft"] = limit;
				*motors["BackRight"] = limit;
				*motors["FrontLeft"] = limit;
				*motors["FrontRight"] = limit;
			} else {
				*motors["BackLeft"] = res;
				*motors["BackRight"] = res;
				*motors["FrontLeft"] = res;
				*motors["FrontRight"] = res;
			}
			time += 20;
			pros::delay(20);
		}
	}

	if (dist < 0) { 

		double kP = 0.245;
		double kI = 0;
		double kD = 0.06;
		double P = 0, I = 0, D = 0;

		while (avgPos > (dist + 5)) {
			if (motors["BackLeft"]->get_actual_velocity() < 2 && time > 1500) {
				break;
			}
			leftPos = motors["BackLeft"]->get_position() - leftOffset;
			rightPos = motors["BackRight"]->get_position() - rightOffset;
			avgPos = (leftPos + rightPos) / 2;


			double err = dist - avgPos;
			P = err;
			I += err;
			D = err - prevErr;
			prevErr = err;

			//double turn = (0.25) * inertialSensor.get_rotation() - turnOffset; //TODO adjust turn correction

			double res = (kP * P) + (kI * I) + (kD * D);
			if ((res < 0 && res < limit) || (res > 0 && res > limit)) {
				moveVel(limit);
			} else {
				moveVel(res);
			}
			time += 20;
			pros::delay(20);
		}
	}
	moveVel(0);
	pros::delay(100);
}

void Robot::moveVel(int vel) {
	*motors["BackLeft"] = vel;
	*motors["BackRight"] = vel;
	*motors["FrontLeft"] = vel;
	*motors["FrontRight"] = vel;
}

void Robot::turn(int degrees) {  // TODO tune values for turn w/ sensor (later with mecanum)
	int time;
	double kP = 1.6;
	double kI = 0;
	double kD = 0.1;
	double P = 0, I = 0, D = 0;

	double offset = inertialSensor.get_rotation();
	double pos = 0;
	double prevErr = degrees; 
	
	if (degrees > 0) { //turning left
		while (pos < (degrees - 5)) {
		// if (motors["BackRight"]->get_actual_velocity() < 2 && time > 500) {
		// 	break;
		// }
		
		pos = inertialSensor.get_rotation() - offset;

		double err = degrees - pos;
		P = err;
		I += err;
		D = err - prevErr;
		prevErr = err;

		double res = (kP * P) + (kI * I) + (kD * D);
		*motors["BackLeft"] = -res;
		*motors["BackRight"] = res;
		*motors["FrontLeft"] = -res;
		*motors["FrontRight"] = res;

		time += 20;
		pros::delay(20);
		}
	}

	if (degrees < 0) { //turning right
		while (pos > (degrees + 5)) {
		// if (motors["BackRight"]->get_actual_velocity() < 2 && time > 500) {
		// 	break;
		// }
		
		pos = inertialSensor.get_rotation() - offset;

		double err = degrees - pos;
		P = err;
		I += err;
		D = err - prevErr;
		prevErr = err;

		double res = (kP * P) + (kI * I) + (kD * D);
		*motors["BackLeft"] = -res;
		*motors["BackRight"] = res;
		*motors["FrontLeft"] = -res;
		*motors["FrontRight"] = res;

		time += 20;
		pros::delay(20);
		}
	}

	
	moveVel(0);
	pros::delay(100);
}

void Robot::strafe(int dist) {
	double time = 0;
	double kP = 0.2;
	double kI = 0;
	double kD = 0;
	double P = 0, I = 0, D = 0;
	double prevErr = dist;

	double pos1, pos2, avgPos;
	if (dist > 0) {
		double leftOffset = motors["FrontLeft"]->get_position();
		double rightOffset = motors["BackRight"]->get_position();
		avgPos = (leftOffset + rightOffset) / 2;

		while (fabs(dist) - 5 > fabs(avgPos) || fabs(dist) + 5 < fabs(avgPos)) {
			if (motors["BackRight"]->get_actual_velocity() < 2 && time > 500) {
			break;
			}

			double leftPos = motors["FrontLeft"]->get_position();
			double rightPos = motors["BackRight"]->get_position();
			avgPos = (leftPos + rightPos) / 2;

			double err = dist - avgPos;
			P = err;
			I += err;
			D = err - prevErr;
			prevErr = err;

			double res = (kP * P) + (kI * I) + (kD * D);

			*motors["BackLeft"] = -res;
			*motors["BackRight"] = res;
			*motors["FrontLeft"] = res;
			*motors["FrontRight"] = -res;

			time += 20;
			pros::delay(20);
		}
	}

	if (dist < 0) {
		dist = -dist;
		double leftOffset = motors["BackLeft"]->get_position();
		double rightOffset = motors["FrontRight"]->get_position();
		avgPos = (leftOffset + rightOffset) / 2;

		while (fabs(dist) - 5 > fabs(avgPos) || fabs(dist) + 5 < fabs(avgPos)) {
			if (motors["BackLeft"]->get_actual_velocity() < 2 && time > 500) {
			break;
			}

			double leftPos = motors["BackLeft"]->get_position();
			double rightPos = motors["FrontRight"]->get_position();

			double err = dist - avgPos;
			P = err;
			I += err;
			D = err - prevErr;
			prevErr = err;

			double res = (kP * P) + (kI * I) + (kD * D);

			*motors["BackLeft"] = res;
			*motors["BackRight"] = -res;
			*motors["FrontLeft"] = -res;
			*motors["FrontRight"] = res;

			time += 20;
			pros::delay(20);
		}
	}
	moveVel(0);
	pros::delay(100);
}

void Robot::diagonal(diag direction, int dist, double angle) {
	int time = 0;
	double kP = 0;
	double kI = 0;
	double kD = 0;

	double P = 0, I = 0, D = 0;

	double leftOffset, rightOffset;
	double leftPos, rightPos, avgPos;
	double err, prevErr = dist, res;

	switch(direction) {
		case diag::forwardRight: 
			leftOffset = motors["FrontLeft"]->get_position();
			rightOffset = motors["BackRight"]->get_position();
			avgPos = (leftOffset + rightOffset) / 2;

			while(fabs(dist) - 5 > fabs(avgPos) || fabs(dist) + 5 < fabs(avgPos)) {
				if (motors["BackRight"]->get_actual_velocity() < 2 && time > 200) {
					break;
				}
				
				leftPos = motors["FrontLeft"]->get_position();
				rightPos = motors["BackRight"]->get_position();

				avgPos = (leftPos + rightPos) / 2;

				err = dist - avgPos;
				P = err;
				I += err;
				D = err - prevErr;

				res = (kP * P) + (kI * I) + (kD * D);

				*motors["FrontLeft"] = res;
				*motors["BackRight"] = res;
				*motors["FrontRight"] = angle * res;
				*motors["BackLeft"] = angle * res;

				time += 20;
				pros::delay(20);
			}
			moveVel(0);
			pros::delay(100);
			break;
		
		case diag::forwardLeft: 
			leftOffset = motors["BackLeft"]->get_position();
			rightOffset = motors["FrontRight"]->get_position();
			avgPos = (leftOffset + rightOffset) / 2;

			while(fabs(dist) - 5 > fabs(avgPos) || fabs(dist) + 5 < fabs(avgPos)) {
				if (motors["BackLeft"]->get_actual_velocity() < 2 && time > 200) {
					break;
				}
				
				leftPos = motors["BackLeft"]->get_position();
				rightPos = motors["FrontRight"]->get_position();

				avgPos = (leftPos + rightPos) / 2;

				err = dist - avgPos;
				P = err;
				I += err;
				D = err - prevErr;

				res = (kP * P) + (kI * I) + (kD * D);

				*motors["BackLeft"] = res;
				*motors["FrontRight"] = res;
				*motors["BackRight"] = angle * res;
				*motors["FrontLeft"] = angle * res;

				time += 20;
				pros::delay(20);
			}
			moveVel(0);
			pros::delay(100);
			break;

		case diag::backwardLeft:
			leftOffset = motors["FrontLeft"]->get_position();
			rightOffset = motors["BackRight"]->get_position();
			avgPos = (leftOffset + rightOffset) / 2;

			while(fabs(dist) - 5 > fabs(avgPos) || fabs(dist) + 5 < fabs(avgPos)) {
				if (motors["BackRight"]->get_actual_velocity() < 2 && time > 200) {
					break;
				}
				
				leftPos = motors["FrontLeft"]->get_position();
				rightPos = motors["BackRight"]->get_position();

				avgPos = (leftPos + rightPos) / 2;

				err = dist - avgPos;
				P = err;
				I += err;
				D = err - prevErr;

				res = (kP * P) + (kI * I) + (kD * D);

				*motors["FrontLeft"] = res;
				*motors["BackRight"] = res;
				*motors["FrontRight"] = angle * res;
				*motors["BackLeft"] = angle * res;
				
				time += 20;
				pros::delay(20);
			}
			moveVel(0);
			pros::delay(100);
			break;

		case diag::backwardRight:
			leftOffset = motors["BackLeft"]->get_position();
			rightOffset = motors["FrontRight"]->get_position();
			avgPos = (leftOffset + rightOffset) / 2;

			while(fabs(dist) - 5 > fabs(avgPos) || fabs(dist) + 5 < fabs(avgPos)) {
				if (motors["BackLeft"]->get_actual_velocity() < 2 && time > 200) {
					break;
				}
				
				leftPos = motors["BackLeft"]->get_position();
				rightPos = motors["FrontRight"]->get_position();

				avgPos = (leftPos + rightPos) / 2;

				err = dist - avgPos;
				P = err;
				I += err;
				D = err - prevErr;

				res = (kP * P) + (kI * I) + (kD * D);

				*motors["BackLeft"] = res;
				*motors["FrontRight"] = res;
				*motors["BackRight"] = angle * res;
				*motors["FrontLeft"] = angle * res;
			
				time += 20;
				pros::delay(20);
			}
			moveVel(0);
			pros::delay(100);
			break;

		default: break;
	}
}

void Robot::anglerSet(int degrees) {
	anglerSetPoint.store(degrees);
}

void Robot::liftSet(int position) {
	liftSetPoint.store(position);
}

int Robot::getLiftSet() {
	return liftSetPoint.load();
}

int Robot::getAnglerSet() {
	return anglerSetPoint.load();
}

void Robot::amoveSet(bool set) {
	amove = set;
}

void Robot::lmoveSet(bool set) {
	lmove = set;
}

void Robot::liftPID(void* params) {
	double kP = 0.25;
	double kI = 0;
	double kD = 0.35;
	double P = 0, I = 0, D = 0;
	double prevErr = sRobot->getLiftSet();
	int lastSet = sRobot->getLiftSet();
	while (true) {
		if (master.get_digital(DIGITAL_Y)) {
			*(sRobot->getMotor("Lift")) = 127;
			master.set_text(0, 0, "lift   up");
		} else if (master.get_digital(DIGITAL_B)) {
			*(sRobot->getMotor("Lift")) = -127;
			master.set_text(0, 0, "lift   down");
		} else if (master.get_digital(DIGITAL_A)) {
			*(sRobot->getMotor("Lift")) = -30;
		} else if (lmove) {
			int currentSet = sRobot->getLiftSet();
			I = 0;

			double err = currentSet - sRobot->getAnalogSensor("Lift Potentiometer")->get_value();
			P = err;
			I += err;
			D = err - prevErr;
			prevErr = err;

			*(sRobot->getMotor("Lift")) = (kP * P) + (kI * I) + (kD * D);
		} else {
			*(sRobot->getMotor("Lift")) = 0;
			sRobot->getMotor("Lift")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		}
		pros::delay(20);
	}
}

void Robot::anglerPID(void* params) {
	
	double kP = 0.2;
	double kI = 0;
	double kD = 0.2;
	double P = 0, I = 0, D = 0;
	double prevErr = sRobot->getAnglerSet();
	int lastSet = sRobot->getAnglerSet();
	while (true) {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 1400) {
				*(sRobot->getMotor("Angler")) = 127;
			} else if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 2200) {
				*(sRobot->getMotor("Angler")) = (0.075) * (2750 - sRobot->getAnalogSensor("Angler Potentiometer")->get_value());
			}
			else if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 2700) {
				*(sRobot->getMotor("Angler")) = (0.05) * (2750 - sRobot->getAnalogSensor("Angler Potentiometer")->get_value());
			}
			else {
				*(sRobot->getMotor("Angler")) = 0;
				sRobot->getMotor("Angler")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			}
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() > 350) {
				*(sRobot->getMotor("Angler")) = -127;
			}
			else {
				*(sRobot->getMotor("Angler")) = 0;
				sRobot->getMotor("Angler")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			}
			
		} else if (amove) {
			I = 0;
			
			double err = sRobot->getAnglerSet() - sRobot->getAnalogSensor("Angler Potentiometer")->get_value();

			P = err;
			I += err;
			D = err - prevErr;
			prevErr = err;

			*(sRobot->getMotor("Angler")) = (kP * P) + (kI * I) + (kD * D);
		} else {
			*(sRobot->getMotor("Angler")) = 0;
			sRobot->getMotor("Angler")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		}
		pros::delay(20);
	}
}

void Robot::drive(void* params) {
	while(true) {		
		int lefty = master.get_analog(ANALOG_LEFT_Y);
		int leftx = master.get_analog(ANALOG_LEFT_X);
		int right = master.get_analog(ANALOG_RIGHT_X);
		int avgPos = (sRobot->getMotor("BackLeft")->get_position() + sRobot->getMotor("BackRight")->get_position()) / 2;
		int power = (int) std::round(127.0 * std::pow((double) lefty / 127, (double) 13 / 9));
		int strafe = (int) std::round(127.0 * std::pow((double) leftx / 127, (double) 25 / 9));
		int turn = (int) std::round(127.0 * std::pow((double) right / 127, (double) 19 / 9));

		if (lefty < 0)
		{
			power = -1 * ((int) std::round(127.0 * std::pow((double) (-lefty) / 127, (double) 13 / 9)));
		}

		if (leftx < 0)
		{
			strafe = -1 * (int) std::round(127.0 * std::pow((double) (-leftx) / 127, (double) 25 / 9));
		}

		if(right < 0)
		{
			turn = -1 * ((int) std::round(127.0 * std::pow((double) (-right) / 127, (double) 19 / 9)));
		}
		
		pros::lcd::set_text(4, "Angler: " + std::to_string(sRobot->getAnalogSensor("Angler Potentiometer")->get_value()));
		pros::lcd::set_text(5, "Lift: " + std::to_string(sRobot->getAnalogSensor("Lift Potentiometer")->get_value()));
		pros::lcd::set_text(6, "avgPos: " + std::to_string(avgPos));
		pros::lcd::set_text(7, "rotation: " + std::to_string(inertialSensor.get_rotation()));

		if (master.get_digital(DIGITAL_A)) {
			sRobot->moveVel(-100);
		} else {
			sRobot->mecanum(power, strafe, turn);
		}

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			sRobot->intake(127);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) || master.get_digital(DIGITAL_A)) {
			sRobot->intake(-127);
		} else {
			sRobot->intakeStop();
		}
		
		pros::delay(20); 
	}
}

void Robot::intake(int speed) {
	*motors["LeftIntake"] = speed;
	*motors["RightIntake"] = speed;
}

void Robot::intakeStop() {
	*motors["LeftIntake"] = 0;
	*motors["RightIntake"] = 0;
}

void Robot::startTask(std::string name, void (*func)(void*), void* params) {
	if (!existsTask(name)) {
		tasks.insert(std::pair<std::string, std::unique_ptr<pros::Task>>(name, std::move(std::make_unique<pros::Task>(func, params, name.c_str()))));
	}
}

void Robot::pauseTask(std::string name) {
	tasks.at(name)->suspend();
}

void Robot::resumeTask(std::string name) {
	tasks.at(name)->resume();
}

void Robot::killTask(std::string name) {
	tasks.at(name)->remove();
	tasks.erase(name);
}

bool Robot::isRunning(std::string name) {
	return tasks.at(name)->get_state() == pros::E_TASK_STATE_RUNNING;
}

bool Robot::existsTask(std::string name) {
	return tasks.find(name) != tasks.end();
}

pros::Motor* Robot::getMotor(std::string name) {
	return motors.at(name).get();
}

pros::ADIAnalogIn* Robot::getAnalogSensor(std::string name) {
	return sensorAnalog.at(name).get();
}

pros::Controller* Robot::getMaster() {
	return controller;
}