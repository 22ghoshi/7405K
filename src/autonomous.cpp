// #include "autonomous.hpp"
 #include "initialize.hpp"
 #include "Robot.hpp"
 #include "main.h"
// #include <vector>
// #include <string>

// /**
//  * Runs the user autonomous code. This function will be started in its own task
//  * with the default priority and stack size whenever the robot is enabled via
//  * the Field Management System or the VEX Competition Switch in the autonomous
//  * mode. Alternatively, this function may be called in initialize or opcontrol
//  * for non-competition testing purposes.
//  *
//  * If the robot is disabled or communications is lost, the autonomous task
//  * will be stopped. Re-enabling the robot will restart the task, not re-start it
//  * from where it left off.
//  */

void topRed() {
    
}

void bottomRed() { //TODO bottomRed auton first
    sRobot->intakeOut(-127);
    pros::delay(1000);
    sRobot->intakeIn();
    sRobot->moveDist(2150, 60);
}

void topBlue() {

}

void bottomBlue() {

}

void test() { //TODO turn pid testing here
    double startPos = sRobot->getMotor("BackLeft")->get_position();
    sRobot->turn(90);
    printf("finished %f\n", (sRobot->getMotor("BackLeft")->get_position() - startPos) - 450);
}

void push() {
    sRobot->intakeOut(-127);
    pros::delay(1000);
    sRobot->intakeStop();
    pros::delay(500);
    sRobot->moveVel(-90);
    pros::delay(750);
    sRobot->moveVel(0);
    pros::delay(750);
    sRobot->moveDist(1500, 127);
}

void autonomous() {
   switch(autonselect)
   {
       case autonSelect::topRed: 
        topRed();
        break;
       case autonSelect::botRed: 
        bottomRed();
        break;
       case autonSelect::topBlue: 
        topBlue();
        break;
       case autonSelect::botBlue: 
        bottomBlue();
        break;
       case autonSelect::test: 
        test();
        break;
       case autonSelect::push: 
        push();
        break;
       default:
        push();
        break;
   }
}