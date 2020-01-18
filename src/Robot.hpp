#pragma once
#include <map>
#include <string>
#include <memory>
#include <atomic>

#include "main.h"

#define sRobot Robot::Instance()

class Robot {
    private:
    Robot();
    Robot(Robot const&){};
    Robot& operator=(Robot const&) {
        return *this;
    };
    std::map<std::string, std::unique_ptr<pros::Motor>> motors;
    std::map<std::string, std::unique_ptr<pros::ADIAnalogIn>> sensorAnalog;
    std::map<std::string, std::unique_ptr<pros::Task>> tasks;
    static Robot* pInstance;
    std::atomic<int> liftSetPoint;
    std::atomic<int> anglerSetPoint;

    int getLiftSet();
    int getAnglerSet();

    public:
    static Robot* Instance();
    void arcade(int left, int right);

    void moveDist(int dist);
    void moveVel(int vel);
    void turn(int degrees);
    void anglerSet(int degrees);
    void liftSet(int position);
    static void anglerPID(void* params);
    static void liftPID(void* params);
    void intakeIn();
    void intakeOut();
    void intakeStop();

    void startTask(std::string name, void (*func)(void*), void* params = nullptr);
    void pauseTask(std::string name);
    void resumeTask(std::string name);
    void killTask(std::string name);
    bool existsTask(std::string name);

    pros::Motor* getMotor(std::string name);
    pros::ADIAnalogIn* getAnalogSensor(std::string name);
};