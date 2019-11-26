#include "main.h"
#include <vector>
#include<string>

using namespace std;





//vector of autons
std::vector<string> autonNames{
	"topRed",
	"botRed",
	"topBlue",
	"botBlue",
	"skills",
	"test"
};



int autonselect = 0;
int totalautons = autonNames.size();



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void on_center_button() 
{
	pros::lcd::set_text(3, "Running Auton " + autonNames[autonselect - 1] + "...");
	pros::delay(1000);
}

void on_right_button()
{
	if (autonselect == totalautons)
	{
		autonselect = 0;
	}
	
	
	autonselect++;
	
	
	pros::lcd::set_text(2, autonNames[autonselect - 1]);
}

void on_left_button()
{

}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Select Auton Below");
	pros::lcd::set_text(2, "[NONE]");

	

	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn2_cb(on_right_button);
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
