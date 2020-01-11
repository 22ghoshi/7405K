#include "main.h"
#include <vector>
#include<string>

using namespace std;



/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

extern int autonselect;
//functions for autons from opcontrol
void move(int destination);
void rightTurn(int turn);
void leftTurn(int turn);
void driveMove(int dist, int speed);
void driveVel(int dist, int speed);
void deploy();
void intake(int set);
void anglerMove(int dist);
void liftMove(int dist);
void backout(int time);

void topRed()
{
    deploy();

    
}

void botRed()
{
    //place bot in front of 3 row
    //deploy
    deploy();
    //liftMove(-750);

    //start
    //intake row of four
    intake(1);
    driveMove(2100, 50);
    pros::delay(700);
    intake(0);

    //drive to corner
    driveMove(-1150, 100);
    rightTurn(900);
    driveVel(1500, 80);
    pros::delay(1000);
    driveMove(-100, 50);

    //stack
    intake(-1);
    anglerMove(5300);
    intake(0);
    driveMove(50, 50);

    //back away
    backout(600);
}

void topBlue()
{

}

void botBlue()
{
    intake(1);
    driveMove(2850, 50);
    pros::delay(800);
    intake(0);
    driveMove(-1800, 85);
    leftTurn(650);
    driveVel(1500, 80);
    pros::delay(1000);
    driveMove(-100, 50);
    intake(-1);
    anglerMove(5300);
    intake(0);
    driveMove(50, 50);
    backout(600);
}

void test()
{
    deploy();
}

void push()
{
    driveMove(-1080, 90);
    driveMove(1080, 90);
    deploy();
}

void autonomous() 
{

    /*minimum  autons - 
    maximum scoring (skills)
    top red / bot red hoarding cubes
    top blue / bot blue hoarding cubes
    */
   pros::lcd::print(6, "%d", autonselect);
   switch(autonselect)
   {
       case 1: topRed();
       break;
       case 2: botRed();
       break;
       case 3: topBlue();
       break;
       case 4: botBlue();
       break;
       case 5: test();
       break;
       case 6: push();
       break;
       default: push();
       break;
   }
}
