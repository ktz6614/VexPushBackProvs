#include "auton_externs.hpp"
#include "odomreset_fixed.hpp"
#include "robot_states.hpp"
extern pros::Distance distLeft;
extern pros::Distance distRight;
extern pros::Controller master;

// 7-block autonomous routines
inline void left7blocks(){
	chassis.setPose(-46.677,14.462,90);
	scoring(Scoring::HOARD);
	chassis.turnToHeading(75, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-16.5,24.5,1400,{.maxSpeed=50},false);
	matchload(Matchload::EXTEND);
	chassis.turnToHeading(315,800,{.maxSpeed=100},false);
	chassis.moveToPoint(-40.447,45.404,2000,{.maxSpeed=120,.minSpeed=10},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(false,true,true);
	chassis.moveToPoint(-62.926,47.404,1550,{.forwards=true,.maxSpeed=120},false);
	resetposition(false,true,true);
	chassis.moveToPoint(-21.183,46.004,1500,{.forwards=false,.maxSpeed=60},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(2200);
	scoring(Scoring::NONE);
	resetposition(false,true,true);
	chassis.moveToPoint(-34.183,36.404,700,{.forwards=true,.maxSpeed=120,.minSpeed=20},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-10.101, 36.404, 1000,{.forwards=false,.maxSpeed=60},false);
}

inline void right7blocks(){
	chassis.setPose(-46.677,-14.462,90);
	scoring(Scoring::HOARD);
	chassis.turnToHeading(115, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-17.5,-26.5,1400,{.maxSpeed=50},false);
	matchload(Matchload::EXTEND);
	chassis.turnToHeading(225,800,{.maxSpeed=100},false);
	chassis.moveToPoint(-35.447,-46.404,2000,{.maxSpeed=100,.minSpeed=50},false);
	chassis.turnToHeading(270, 1000,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	scoring(Scoring::HOARD);
	resetposition(true,false,false);
	chassis.moveToPoint(-57.926,-46.404,1700,{.forwards=true,.maxSpeed=120,.minSpeed=50},false);
	resetposition(true,false,false);
	chassis.moveToPoint(-21.183,-46.404,1500,{.forwards=false,.maxSpeed=80,.minSpeed=50},false);
	scoring(Scoring::LONG_GOAL);
	matchload(Matchload::RETRACT);
	pros::delay(2200);
	scoring(Scoring::NONE);
	resetposition(true,false,false);
	chassis.moveToPoint(-34.183,-57.604,650,{.forwards=true,.maxSpeed=120,.minSpeed=50},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-10.101, -57.604, 1000,{.forwards=false,.maxSpeed=60,.minSpeed=50},false);
}
