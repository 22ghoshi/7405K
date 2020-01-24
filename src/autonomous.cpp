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
    
//     //work here
}

void bottomRed() {
//     //place bot in front of 3 row
//     //deploy
//     deploy();
//     //liftsRobot->moveDist(-750);


//     //for 8 cube testing later
//     /*

//     intake(1);
//     sRobot->sRobot->moveDist(2100);
//     sRobot->sRobot->moveDist(-300);
//     sRobot->sRobot->moveDist(300);
//     leftTurn(300);
//     sRobot->sRobot->moveDist(-3000);
//     rightTurn(300);
//     //continue on

//     */

//     //start for now
//     //intake row of four
//     intake(1);
//     sRobot->sRobot->moveDist(2100, 50);
    
//     intake(0);

//     //drive to corner
//     sRobot->sRobot->moveDist(-1150, 100);
//     rightTurn(900);
//     driveVel(1500, 80);
//     pros::delay(1000);
//     sRobot->sRobot->moveDist(-100, 50);

//     //stack
//     intake(-1);
//     anglersRobot->moveDist(3600);
//     intake(0);
//     sRobot->moveDist(50, 50);

//     //back away
//     backout(600);
}

void topBlue() {
//     //and here
}

void bottomBlue() {
//     intake(1);
//     sRobot->moveDist(2850);
//     pros::delay(800);
//     intake(0);
//     sRobot->sRobot->moveDist(-1800);
//     sRobot->turn(650);
//     sRobot->moveDist(1500);
//     pros::delay(1000);
//     sRobot->moveDist(-100);
//     intake(-1);
//     ->moveDist(5300);
//     intake(0);
//     sRobot->sRobot->moveDist(50, 50);
//     backout(600);
}

void test() { //TODO botRed auton here
    double startPos = sRobot->getMotor("BackLeft")->get_position();
    sRobot->intakeOut();
    pros::delay(1000);
    sRobot->intakeIn();
    sRobot->moveDist(2150, 0.2);
    pros::delay(500);
    sRobot->intakeStop();
    sRobot->moveDist(-1500, 0.5);
    sRobot->turn(100);
    pros::delay(1000);
    printf("finished %f\n", (sRobot->getMotor("BackLeft")->get_position() - startPos) - 450);
}

void push() {
//     sRobot->moveDist(-1080, 90);
//     sRobot->moveDist(1080, 90);
//     deploy();
}

// void backout() {
    
// }


void autonomous() {

    /*minimum  autons - 
    maximum scoring (skills)
    top red / bot red hoarding cubes
    top blue / bot blue hoarding cubes
    */
   
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