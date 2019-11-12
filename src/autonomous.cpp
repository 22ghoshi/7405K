#include "main.h"
#include<bits/stdc++.h> 
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

//functions from opcontrol
double pid(double error);
void move();
void rightTurn(int turn);
void leftTurn(int turn);
void anglerShift(int set); //0 for intaking, 1 for stacking

void topRed(int x)
{

}

void botRed(int x)
{

}

void topBlue(int x)
{

}

void botBlue(int x)
{

}

void skills(int x)
{
    
}

void autonomous() 
{

    /*minimum  autons - 
    maximum scoring (skills)
    top red / bot red hoarding cubes
    top blue / bot blue hoarding cubes
    maybe towers autons for all four
    */
}
