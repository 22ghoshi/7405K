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
void driveMove(int dist);
void driveMove(int dist, int speed);
void intake(int set);
void anglerMove(int dist);
void backout(int time);

void topRed()
{
    
}

void botRed()
{
    //place bot in front of four row, put preload in front of it
    //deploy
    intake(-1);
    pros::delay(1250);
    intake(0);

    //intake row of five (w/ preload)
    intake(1);
    driveMove(2500, 60);
    intake(0);

    //move back and turn to face corner
    driveMove(-1750, -100);
    rightTurn(1500);

    //move to corner and angle stack vertical
    driveMove(500);
    anglerMove(1195);
    intake(-1);
    pros::delay(500);
    intake(0);

    //back away
    backout(1000);
}

void topBlue()
{

}

void botBlue()
{
    //place bot in front of 3 row
    //deploy
    intake(-1);
    pros::delay(1250);
    intake(0);

    //intake row
    intake(1);
    driveMove(1750);
    driveMove(-200);
    driveMove(400);
    intake(0);

    //angle towards right, move back, align with four row
    rightTurn(400);
    driveMove(-3000);
    leftTurn(400);

    //intake row of four
    intake(1);
    driveMove(2250);
    intake(0);

    //back up and move to score
    driveMove(-2000);
    leftTurn(1500);
    driveMove(500);

    //score
    driveMove(500);
    anglerMove(1195);
    intake(-1);
    pros::delay(500);
    intake(0);

    //back away
    backout(1000);
}

void test()
{
    intake(1);
    driveMove(540, 50);
    intake(0);
    opcontrol();
}

void push()
{
    driveMove(-1080);
    driveMove(1080);
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
