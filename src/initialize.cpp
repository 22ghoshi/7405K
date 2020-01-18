#include "main.h"
#include "initialize.hpp"
#include "autonomous.hpp"
#include "Robot.hpp"
#include <vector>
#include <string>
#include <memory>

//vector of autons
std::vector<std::string> autonNames {
	"topRed",
	"botRed",
	"topBlue",
	"botBlue",
	"test",
	"push"
};


autonSelect autonselect = autonSelect::push;
int totalautons = autonNames.size();

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void on_center_button() 
{
	pros::lcd::set_text(3, "Running Auton " + autonNames[(int)autonselect - 1] + "...");
	//pros::lcd::set_text(6, std::to_string(angler.get_position()));
	pros::delay(1000);
}

void on_right_button()
{
	if ((int)autonselect == totalautons)
	{
		autonselect = autonSelect::push;
	}
	
	
	autonselect = (autonSelect)((int)autonselect + 1);
	
	
	pros::lcd::set_text(2, autonNames[(int)autonselect - 1]);
}

void on_left_button()
{

}

void initialize() {
	pros::lcd::initialize();
	pros::delay(20);
	pros::lcd::set_text(1, "Select Auton Below");
	pros::lcd::set_text(2, autonNames[(int)autonselect - 1] + " ");
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn2_cb(on_right_button);
	
	// Robot Class and PID init
	Robot::Instance();
	sRobot->startTask("Angler PID", Robot::anglerPID);
	sRobot->startTask("Lift PID", Robot::liftPID);
}



/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
