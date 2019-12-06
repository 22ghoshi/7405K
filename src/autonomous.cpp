#include "main.h"
#include <vector>
#include<string>

using namespace std;

extern pros::Motor left_intake;
extern pros::Motor right_intake;
extern pros::Motor angler;
extern pros::Motor backleft_mtr;
extern pros::Motor backright_mtr;
extern pros::Motor frontleft_mtr;
extern pros::Motor frontright_mtr;

void intake(int set)
{
    if(set == 1)
    {
        left_intake = 127;
        right_intake = -127; 
    }
    else if(set == 0)
    {
        left_intake = 0;
        right_intake = 0; 
    }
    else if(set ==--1)
    {
        left_intake = -40;
        right_intake = 40;
    }
}

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
double pid(double error);
void move();
void rightTurn(int turn);
void leftTurn(int turn);
void driveMove(int dist);
void driveMove(int dist, int speed);

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
    angler.move_absolute(1195, 55);
    while(angler.get_position() < 1193)
    {
        pros::delay(2);
    }
    intake(-1);
    pros::delay(500);
    intake(0);

    //back away
    backleft_mtr = -50;
	backright_mtr = 50;
	frontleft_mtr = -50;
	frontright_mtr = 50;
    left_intake = -85;
    right_intake = 85;
    pros::delay(500);
    backleft_mtr = 0;
	backright_mtr = 0;
	frontleft_mtr = 0;
	frontright_mtr = 0;
    left_intake = 0;
    right_intake = 0;
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
    angler.move_absolute(1195, 55);
    while(angler.get_position() < 1193)
    {
        pros::delay(2);
    }
    intake(-1);
    pros::delay(500);
    intake(0);

    //back away
    backleft_mtr = -50;
	backright_mtr = 50;
	frontleft_mtr = -50;
	frontright_mtr = 50;
    left_intake = -85;
    right_intake = 85;
    pros::delay(500);
    backleft_mtr = 0;
	backright_mtr = 0;
	frontleft_mtr = 0;
	frontright_mtr = 0;
    left_intake = 0;
    right_intake = 0;
}

void skills()
{
    driveMove(-1080);
    driveMove(1080);
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
       case 5: skills();
       break;
       case 6: push();
       break;
       default: push();
       break;
   }
}
