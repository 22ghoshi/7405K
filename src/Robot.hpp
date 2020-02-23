#pragma once
#include <atomic>
#include <map>
#include <memory>
#include <string>

#include "main.h"


#define sRobot Robot::Instance()

enum diag { forwardRight = 1, forwardLeft, backwardRight, backwardLeft};

class Robot {
	private:
	Robot();
	Robot(Robot const&){};
	Robot& operator=(Robot const&) {
		return *this;
	};
	std::atomic<int> liftSetPoint;
	std::atomic<int> anglerSetPoint;
	std::map<std::string, std::unique_ptr<pros::Motor>> motors;
	std::map<std::string, std::unique_ptr<pros::ADIAnalogIn>> sensorAnalog;
	std::map<std::string, std::unique_ptr<pros::Task>> tasks;

	static Robot* pInstance;
	static pros::Controller* controller;

	static bool amove;
	static bool lmove;

	public:
	int getLiftSet();
	int getAnglerSet();
	static Robot* Instance();
	void mecanum(int power, int strafe, int turn);

	void moveDist(int dist, int limit);
	void moveVel(int vel);
	void turn(int degrees);
	void strafe(int dist);
	void diagonal(diag direction, int dist, double angle);
	void anglerSet(int degrees);
	void liftSet(int position);
	void amoveSet(bool set);
	void lmoveSet(bool set);
	static void anglerPID(void* params);
	static void liftPID(void* params);
	static void drive(void* params);
	static void taskController(void* params);
	void intake(int speed);
	void intakeStop();

	void startTask(std::string name, void (*func)(void*), void* params = nullptr);
	void pauseTask(std::string name);
	void resumeTask(std::string name);
	void killTask(std::string name);
	bool isRunning(std::string name);
	bool existsTask(std::string name);

	pros::Motor* getMotor(std::string name);
	pros::ADIAnalogIn* getAnalogSensor(std::string name);
	pros::Controller* getMaster();
};