#include "Robot.hpp"

#define ROT 1800 / 360

Robot* Robot::pInstance = NULL;

Robot::Robot() {
	// Starting Values for setpoints
	liftSetPoint.store(60);
	anglerSetPoint.store(990);

	// Motors Init
	motors["BackLeft"] = std::make_unique<pros::Motor>(3);
	motors["BackRight"] = std::make_unique<pros::Motor>(6, true);
	motors["FrontLeft"] = std::make_unique<pros::Motor>(2);
	motors["FrontRight"] = std::make_unique<pros::Motor>(4, true);
	motors["Angler"] = std::make_unique<pros::Motor>(11);
	motors["Lift"] = std::make_unique<pros::Motor>(5);
	motors["LeftIntake"] = std::make_unique<pros::Motor>(10);
	motors["RightIntake"] = std::make_unique<pros::Motor>(9, true);

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
	double accelSpeed = 0;

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
	moveVel(0);
}

void Robot::moveVel(int vel) {
	*motors["BackLeft"] = vel;
	*motors["BackRight"] = vel;
	*motors["FrontLeft"] = vel;
	*motors["FrontRight"] = vel;
}

void Robot::turn(int degrees) {  // TODO tune values for turn
	double kP = 0.6;
	double kI = 0.0;
	double kD = 0.0;
	double P = 0, I = 0, D = 0;
	degrees *= ROT;  // TODO tune this too

	double leftOffset = motors["BackLeft"]->get_position();
	double rightOffset = motors["BackRight"]->get_position();
	double leftPos, rightPos, avgPos = (leftOffset - rightOffset) / 2;
	double prevErr = degrees;

	while (fabs(degrees) - 2 > fabs(avgPos) || fabs(degrees) + 2 < fabs(avgPos)) {
		leftPos = motors["BackLeft"]->get_position() - leftOffset;
		rightPos = motors["BackRight"]->get_position() - rightOffset;
		avgPos = (leftPos - rightPos) / 2;

		double err = degrees - avgPos;
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
}

// void Robot::anglerSet(int degrees) {
// 	anglerSetPoint.store(degrees);
// }

// void Robot::liftSet(int position) {
// 	liftSetPoint.store(position);
// }

// int Robot::getLiftSet() {
// 	return liftSetPoint.load();
// }

// int Robot::getAnglerSet() {
// 	return anglerSetPoint.load();
// }

// void Robot::liftPID(void* params) {  // TODO make lift task
// 	double kP = 0;
// 	double kI = 0;
// 	double kD = 0;
// 	double P = 0, I = 0, D = 0;
// 	double prevErr = sRobot->getLiftSet();
// 	int lastSet = sRobot->getLiftSet();
// 	while (true) {
// 		int currentSet = sRobot->getLiftSet();
// 		if (currentSet != lastSet) {
// 			I = 0;
// 			prevErr = currentSet;
// 		}
// 		double err = currentSet - sRobot->getAnalogSensor("Lift Potentiometer")->get_value();
// 		P = err;
// 		I += err;
// 		D = err - prevErr;
// 		prevErr = err;

// 		*(sRobot->getMotor("Lift")) = (kP * P) + (kI * I) + (kD * D);

// 		pros::delay(20);
// 	}
// }

// void Robot::anglerPID(void* params) {  // TODO make angler task
// 	double kP = 0;
// 	double kI = 0;
// 	double kD = 0;
// 	double P = 0, I = 0, D = 0;
// 	double prevErr = sRobot->getAnglerSet();
// 	int lastSet = sRobot->getAnglerSet();
// 	while (true) {
// 		int currentSet = sRobot->getAnglerSet();
// 		if (currentSet != lastSet) {
// 			I = 0;
// 			prevErr = currentSet;
// 		}
// 		double err = currentSet - sRobot->getAnalogSensor("Angler Potentiometer")->get_value();
// 		P = err;
// 		I += err;
// 		D = err - prevErr;
// 		prevErr = err;

// 		*(sRobot->getMotor("Angler")) = (kP * P) + (kI * I) + (kD * D);

// 		pros::delay(20);
// 	}
// }

void Robot::liftMove(int dist) {
	double time = 0;
	double kP = 0.3;
	double kI = 0.0;
	double kD = 0.35;
	double P = 0, I = 0, D = 0;
	double prevErr = sensorAnalog["Lift Potentiometer"]->get_value();
	double pos = sensorAnalog["Lift Potentiometer"]->get_value();
	while (pos < dist) {
		if (motors["Lift"]->get_actual_velocity() < 2 && time > 200) {
			break;
		}
		pos = sensorAnalog["Lift Potentiometer"]->get_value();
		double err = dist - pos;
		P = err;
		I += err;
		D = err - prevErr;
		prevErr = err;

		*motors["Lift"] = (kP * P) + (kI * I) + (kD * D);
		time += 20;
		pros::delay(20);
	}
}

void Robot::anglerMove(int dist) {
	double time = 0;
	double kP = 0.2;
	double kI = 0.0;
	double kD = 0;
	double P = 0, I = 0, D = 0;
	double prevErr = sensorAnalog["Angler Potentiometer"]->get_value();
	double pos = sensorAnalog["Angler Potentiometer"]->get_value();
	while (pos < dist) {
		if (motors["Angler"]->get_actual_velocity() < 2 && time > 200) {
			break;
		}
		pos = sensorAnalog["Angler Potentiometer"]->get_value();
		double err = dist - pos;
		P = err;
		I += err;
		D = err - prevErr;
		prevErr = err;

		*motors["Angler"] = (kP * P) + (kI * I) + (kD * D);
		time += 20;
		pros::delay(20);
	}
	*motors["Angler"] = 0;
}

void Robot::tower(int tower) {
	int height;
	if (tower == 1) {
		height = 1053;
	} else if (tower == 2) {
		height = 1520;
	}

	if (sensorAnalog["Angler Potentiometer"]->get_value() < 1800) {
		*motors["Angler"] = 127;
	}
	pros::delay(350);
	double time = 0;
	double kP = 0.3;
	double kI = 0.0;
	double kD = 0.35;
	double P = 0, I = 0, D = 0;
	double prevErr = sensorAnalog["Lift Potentiometer"]->get_value();
	double pos = sensorAnalog["Lift Potentiometer"]->get_value();
	while (pos < (height - 5)) {
		if (motors["Lift"]->get_actual_velocity() < 2 && time > 200) {
			break;
		}

		if (sensorAnalog["Angler Potentiometer"]->get_value() < 2000) {
			*motors["Angler"] = 127;
		} else {
			*motors["Angler"] = 0;
		}
		pos = sensorAnalog["Lift Potentiometer"]->get_value();
		double err = height - pos;
		P = err;
		I += err;
		D = err - prevErr;
		prevErr = err;

		*motors["Lift"] = (kP * P) + (kI * I) + (kD * D);
		time += 20;
		pros::delay(20);
	}
	*motors["Angler"] = 0;
	*motors["Lift"] = 0;
}

void Robot::down() {
	double time = 0;
	while (sensorAnalog["Lift Potentiometer"]->get_value() > 50) {
		if (motors["Lift"]->get_actual_velocity() < 2 && time > 500) {
			break;
		}
		*motors["Lift"] = -127;
		time += 20;
		pros::delay(20);
	}
	time = 0;
	while (sensorAnalog["Angler Potentiometer"]->get_value() > 1170) {
		if (motors["Angler"]->get_actual_velocity() < 2 && time > 200) {
			break;
		}
		*motors["Angler"] = -127;
		time += 20;
		pros::delay(20);
	}
}

void Robot::intakeIn() {
	*motors["LeftIntake"] = 127;
	*motors["RightIntake"] = 127;
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

bool Robot::existsTask(std::string name) {
	return tasks.find(name) != tasks.end();
}

pros::Motor* Robot::getMotor(std::string name) {
	return motors.at(name).get();
}

pros::ADIAnalogIn* Robot::getAnalogSensor(std::string name) {
	return sensorAnalog.at(name).get();
}