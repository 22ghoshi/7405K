#include "Robot.hpp"

Robot* Robot::pInstance = NULL;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Imu inertialSensor(15);
bool Robot::tower = false;
bool Robot::stack = false;

Robot::Robot() {
	// Starting Values for setpoints
	liftSetPoint.store(5);
	anglerSetPoint.store(11);
	inertialSensor.reset();
	// Motors Init
	motors["BackLeft"] = std::make_unique<pros::Motor>(3);
	motors["BackRight"] = std::make_unique<pros::Motor>(6, true);
	motors["FrontLeft"] = std::make_unique<pros::Motor>(2);
	motors["FrontRight"] = std::make_unique<pros::Motor>(4, true);
	motors["Angler"] = std::make_unique<pros::Motor>(11);
	motors["Lift"] = std::make_unique<pros::Motor>(14);
	motors["LeftIntake"] = std::make_unique<pros::Motor>(5, pros::E_MOTOR_GEARSET_36);
	motors["RightIntake"] = std::make_unique<pros::Motor>(9, pros::E_MOTOR_GEARSET_36, true);

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
	pros::delay(100);
}

void Robot::moveVel(int vel) {
	*motors["BackLeft"] = vel;
	*motors["BackRight"] = vel;
	*motors["FrontLeft"] = vel;
	*motors["FrontRight"] = vel;
}

void Robot::turn(int degrees) {  // TODO tune values for turn w/ sensor
	double kP = 1;
	double kI = 0.0;
	double kD = 0.2;
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
			if (currentSet != lastSet) {
				I = 0;
				prevErr = currentSet;
			}
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
			if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 500) {
				*(sRobot->getMotor("Angler")) = 127;
			} else if (sRobot->getAnalogSensor("Angler Potentiometer")->get_value() < 1300) {
				*(sRobot->getMotor("Angler")) = 30 + (0.09) * (1300 - sRobot->getAnalogSensor("Angler Potentiometer")->get_value());
			}
			else {
				*(sRobot->getMotor("Angler")) = 0;
				sRobot->getMotor("Angler")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			}
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			*(sRobot->getMotor("Angler")) = -127;
		} else if (tower || stack) {
			int currentSet = sRobot->getAnglerSet();
			if (currentSet != lastSet) {
				I = 0;
				prevErr = currentSet;
			}
			double err = currentSet - sRobot->getAnalogSensor("Angler Potentiometer")->get_value();
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

void Robot::drive(void* params) { //TODO add curve
	while(true) {
		
		
		int left = (int) std::round(127.0 * std::pow((double) master.get_analog(ANALOG_LEFT_Y) / 100, (double) 13 / 9));
		int right = (int) std::round(127.0 * std::pow((double) master.get_analog(ANALOG_RIGHT_X) / 100, (double) 13 / 9));
		pros::lcd::set_text(4, "backleft: " + std::to_string(sRobot->getMotor("BackLeft")->get_position()));
		pros::lcd::set_text(5, "Angler: " + std::to_string(sRobot->getAnalogSensor("Angler Potentiometer")->get_value()));
		pros::lcd::set_text(6, "Angler Set: " + std::to_string(sRobot->getAnglerSet()));
		pros::lcd::set_text(7, "Inertial Sensor:" + std::to_string(inertialSensor.get_rotation()));
		if (master.get_digital(DIGITAL_A)) {
			sRobot->moveVel(-65);
		} else {
			sRobot->arcade(left, right);
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

void Robot::taskController(void* params) {

	sRobot->startTask("Drive", Robot::drive);
	sRobot->startTask("Lift PID", Robot::liftPID);
	if (!(sRobot->existsTask("Angler PID"))) {
		sRobot->startTask("Angler PID", Robot::anglerPID);
	}
	if (!(sRobot->isRunning("Angler PID"))) {
		sRobot->resumeTask("Angler PID");
	}
	
	while(true) {
		if (master.get_digital(DIGITAL_RIGHT)) {
			tower = true;
			sRobot->anglerSet(200);
			pros::delay(500);
			sRobot->liftSet(1053);
			pros::delay(1500);
			tower = false;
		}
		if (master.get_digital(DIGITAL_UP)) {
			tower = true;
			sRobot->anglerSet(200);
			pros::delay(500);
			sRobot->liftSet(1520);
			pros::delay(1500);
			tower = false;
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