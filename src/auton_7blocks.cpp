#include "auton_externs.hpp"
#include "auton_7blocks.hpp"
#include "odomreset_fixed.hpp"
#include "pros/rtos.hpp"
#include "robot_states.hpp"
extern pros::Distance distLeft;
extern pros::Distance distRight;
extern pros::Controller master;

// 7-block autonomous routines
void left7blocks(){
	pros::Task antijam_task(antijam);
	chassis.setPose(-46.677,14.462,90);
	scoring(Scoring::HOARD);
	chassis.turnToHeading(75, 300,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-16.5,24.5,1400,{.maxSpeed=50},true);
	pros::delay(800);
	matchload(Matchload::EXTEND);
	chassis.turnToHeading(315,800,{.maxSpeed=120},false);
	chassis.moveToPoint(-40.447,48.404,2000,{.maxSpeed=120},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=120},false);
	resetposition(false,true,true);
	chassis.moveToPoint(-62.926,47.004,1550,{.forwards=true,.maxSpeed=120},false);
	resetposition(false,true,true);
	chassis.moveToPoint(-21.183,47.004,1500,{.forwards=false,.maxSpeed=120},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(2200);
	scoring(Scoring::NONE);
	resetposition(false,true,true);
	chassis.moveToPoint(-40.183,35.404,1000,{.forwards=true,.maxSpeed=120,.minSpeed=20},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-10.101, 35.404, 1000,{.forwards=false,.maxSpeed=60},false);
	antijam_task.remove();
}

void right7blocks(){
	pros::Task antijam_task(antijam);
	chassis.setPose(-46.677,-14.462,90);
	scoring(Scoring::HOARD);
	chassis.turnToHeading(115, 300,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=120},false);
	chassis.moveToPoint(-17.7,-27.5,1300,{.maxSpeed=50},true);
	pros::delay(800);
	matchload(Matchload::EXTEND);
	chassis.turnToHeading(225,800,{.maxSpeed=120},false);
	chassis.moveToPoint(-38.447,-48.404,1300,{.maxSpeed=127},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=120},false);
	scoring(Scoring::HOARD);
	resetposition(true,false,false);
	chassis.moveToPoint(-60.926,-46.404,1800,{.forwards=true,.maxSpeed=120},false);
	resetposition(true,false,false);
	chassis.moveToPoint(-21.183,-46.404,1500,{.forwards=false,.maxSpeed=120},false);
	scoring(Scoring::LONG_GOAL);
	matchload(Matchload::RETRACT);
	pros::delay(2200);
	scoring(Scoring::NONE);
	resetposition(true,false,false);
	chassis.moveToPoint(-34.183,-59.604,1000,{.forwards=true,.maxSpeed=120},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-10.101, -53.604, 1000,{.forwards=false,.maxSpeed=60},false);
	antijam_task.remove();
}
