#include "Robot.hpp"

Robot* Robot::pInstance = NULL;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller* Robot::controller =  &master;
pros::Imu inertialSensor(15);
bool Robot::tower = false;
bool Robot::stack = false;

Robot::Robot() {
	// Starting Values for setpoints
	liftSetPoint.store(5);
	anglerSetPoint.store(11);
	inertialSensor.reset();
	// Motors Init
	motors["BackLeft"] = std::make_unique<pros::Motor>(6);
	motors["BackRight"] = std::make_unique<pros::Motor>(3, true);
	motors["FrontLeft"] = std::make_unique<pros::Motor>(4);
	motors["FrontRight"] = std::make_unique<pros::Motor>(2, true);
	motors["Angler"] = std::make_unique<pros::Motor>(14);
	motors["Lift"] = std::make_unique<pros::Motor>(12);
	motors["LeftIntake"] = std::make_unique<pros::Motor>(5, pros::E_MOTOR_GEARSET_36);
	motors["RightIntake"] = std::make_unique<pros::Motor>(13, pros::E_MOTOR_GEARSET_36, true);

	// Sensors Init
	sensorAnalog["Angler Potentiometer"] = std::make_unique<pros::ADIAnalogIn>(1);
	sensorAnalog["Lift Potentiometer"] = std::make_unique<pros::ADIAnalogIn>(2);



	pros::delay(50);
}

Robot* Robot::Instance() {
	if (!pInstance) {
		pInstance = new Robot();
	}
	return pInstance;
}

void Robot::arcade(int left, int right) {
	*motors["BackLeft"] = (left + right);
	*motors["BackRight"] = (left - right);
	*motors["FrontLeft"] = (left + right);
	*motors["FrontRight"] = (left - right);
}

void Robot::mecanum(int power, int strafe, int turn) {
	*motors["BackLeft"] = power - strafe + turn;
	*motors["BackRight"] = power + strafe - turn;
	*motors["FrontLeft"] = power + strafe + turn;
	*motors["FrontRight"] = power - strafe - turn;
}

void Robot::moveDist(int dist, int limit) {
	double time = 0;
	double kP = 0.145;
	double kI = 0;
	double kD = 0.017;
	double P = 0, I = 0, D = 0;

	double leftOffset = motors["BackLeft"]->get_position();
	double rightOffset = motors["BackRight"]->get_position();
	double leftPos, rightPos, avgPos = (leftOffset + rightOffset) / 2;
	double prevErr = dist;

	while (fabs(dist) - 5 > fabs(avgPos) || fabs(dist) + 5 < fabs(avgPos)) {
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
			moveVel(limit);
		} else {
			moveVel(res);
		}
		time += 20;
		pros::delay(20);
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
	double kP = 1.15;
	double kI = 0.001;
	if (degrees < 40 && degrees > -40) {
		kI = 0.065;
	}
	double kD = 0.25;
	double P = 0, I = 0, D = 0;

	double offset = inertialSensor.get_rotation();
	double pos = 0;
	double prevErr = degrees;

	while (fabs(degrees) - 2 > fabs(pos) || fabs(degrees) + 2 < fabs(pos)) {
		pos = inertialSensor.get_rotation() - offset;

		double err = degrees - pos;
		P = err;
		I += err;
		D = err - prevErr;
		prevErr = err;

		double res = (kP * P) + (kI * I) + (kD * D);
		*motors["BackLeft"] = res;
		*motors["BackRight"] = -res;
		*motors["FrontLeft"] = res;
		*motors["FrontRight"] = -res;

		pros::delay(20);
	}
	moveVel(0);
	pros::delay(100);
}

void Robot::strafe(int dist) {
	int time;
	double kP = 0;
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

	if (dist > 0) {
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

void Robot::stackSet(bool set) {
	stack = set;
}

void Robot::towerSet(bool set) {
	tower = set;
}

void Robot::liftPID(void* params) {  // TODO tune lift pid
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
		} else if (tower) {
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
		}
		pros::delay(20);
	}
}

void Robot::anglerPID(void* params) {  // TODO tune angler pid
	
	double kP = 0.2;
	double kI = 0;
	double kD = 0.2;
	double P = 0, I = 0, D = 0;
	double prevErr = sRobot->getAnglerSet();
	int lastSet = sRobot->getAnglerSet();
	while (true) {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 2200) {
				*(sRobot->getMotor("Angler")) = 127;
			} else if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 3500) {
				*(sRobot->getMotor("Angler")) = 30 + (0.04) * (3500 - sRobot->getAnalogSensor("Angler Potentiometer")->get_value());
			}
			else {
				*(sRobot->getMotor("Angler")) = 0;
				sRobot->getMotor("Angler")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			}
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			*(sRobot->getMotor("Angler")) = -127;
		} else if (tower || stack) {
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

void Robot::drive(void* params) { //TODO mecanum later
	while(true) {		
		int lefty = master.get_analog(ANALOG_LEFT_Y);
		//int leftx = master.get_analog(ANALOG_LEFT_X);
		int right = master.get_analog(ANALOG_RIGHT_X);
		int avgPos = (sRobot->getMotor("BackLeft")->get_position() + sRobot->getMotor("BackRight")->get_position()) / 2;
		int power = (int) std::round(127.0 * std::pow((double) lefty / 127, (double) 13 / 9));
		//int strafe = (int) std::round(127.0 * std::pow((double) leftx / 127, (double) 13 / 9));
		int turn = (int) std::round(127.0 * std::pow((double) right / 127, (double) 13 / 9));

		if (lefty < 0)
		{
			power = -1 * ((int) std::round(127.0 * std::pow((double) (-lefty) / 127, (double) 13 / 9)));
		}

		// if (leftx < 0)
		// {
		// 	strafe = -1 * (int) std::round(127.0 * std::pow((double) (-leftx) / 127, (double) 13 / 9));
		// }

		if(right < 0)
		{
			turn = -1 * ((int) std::round(127.0 * std::pow((double) (-right) / 127, (double) 13 / 9)));
		}
		
		pros::lcd::set_text(4, "rotation " + std::to_string(inertialSensor.get_rotation()));
		pros::lcd::set_text(5, "avgPos: " + std::to_string(avgPos));
		pros::lcd::set_text(6, "angler: " + std::to_string(sRobot->getAnalogSensor("Angler Potentiometer")->get_value()));

		if (master.get_digital(DIGITAL_A)) {
			sRobot->moveVel(-65);
		} else {
			sRobot->arcade(power, turn);
		}

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			sRobot->intakeIn(127);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) || master.get_digital(DIGITAL_A)) {
			sRobot->intakeOut(-127);
		} else {
			sRobot->intakeStop();
		}
		
		pros::delay(20); 
	}
}

void Robot::intakeIn(int speed) {
	*motors["LeftIntake"] = speed;
	*motors["RightIntake"] = speed;
}

void Robot::intakeOut(int speed) {
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