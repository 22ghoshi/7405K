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

//functions for autons from opcontrol
double pid(double error);
void move();
void rightTurn(int turn);
void leftTurn(int turn);
void anglerShift(int set); //0 for intaking, 1 for stacking
void intake(int set); //0 to stop, 1 for intaking

void topRed()
{
    anglerShift(1);
}

void botRed()
{
    //need to flip out intake/tray at beginning
    
    //align with closest row
    //7 = value needed
    /*
    rightTurn(7);
    move(7);
    leftTurn(7);
    //start intake and intake row
    intake(1);
    move(7); //move until end line
    //move down and intake singular orange cube near end line
    rightTurn(7);
    move(7);
    rightTurn(7);
    //move across end line and align with bottom row
    move(7);
    rightTurn(7);
    //intake row and go all the way to wall 
    move(7);
    intake(0);
    //move to scoring zone
    leftTurn(7);
    move(7);
    //angler
    anglerShift(1);
    //move back from stack
    move(-7);
    anglerShift(0);
    */

    intake(1);
    move(100);
    move(-90);
    intake(0);
    rightTurn(100);
    move(50);
    anglerShift(1);
    pros::delay(1000);
    move(-10);
    
   
}

void topBlue()
{

}

void botBlue()
{
    //flip out intake/tray

    //align with bottom row
    leftTurn(7);
    move(7);
    rightTurn(7);
    //start intake and intake row
    intake(1);
    move(7); //move until end line
    //move up and intake singular green cube near end line
    rightTurn(7);
    move(7);
    rightTurn(7);
    //move across and intake second bottom row
    move(7);
    rightTurn(7);
    //intake row and go all the way to wall
    move(7);
    intake(0);
    //move to scoring zone
    rightTurn(7);
    move(7);
    //angler
    anglerShift(1);
    //move back from stack
    move(-7);
    anglerShift(0);
}

void skills()
{
    
}

void test()
{
    anglerShift(1);
}

void autonomous() 
{

    /*minimum  autons - 
    maximum scoring (skills)
    top red / bot red hoarding cubes
    top blue / bot blue hoarding cubes
    */
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
       case 5: skills();
       break;
       case 6: test();
       break;
       default: test();
       break;
   }
}
