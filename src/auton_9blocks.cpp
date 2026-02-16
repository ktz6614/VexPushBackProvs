#include "auton_externs.hpp"
#include "odomreset_fixed.hpp"
#include "robot_states.hpp"
// 9-block autonomous routines
void left9blocks(){
	chassis.setPose(-48.677,14.462,90);
	scoring(Scoring::HOARD);
	chassis.turnToHeading(70, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},true);
	chassis.moveToPoint(-17.5,25.5,2000,{.maxSpeed=80,.minSpeed=20},false);
	chassis.turnToHeading(50, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},true);
	chassis.moveToPoint(-6.761, 42.175, 2000,{.maxSpeed=60,.minSpeed=20},false);
	matchload(Matchload::EXTEND);
	pros::delay(500);
	chassis.moveToPoint(-19.711,40.894,2000,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	chassis.moveToPoint(-41.985, 48.404, 2000,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(true,true);
	chassis.moveToPoint(-21.183,46.404,1300,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-62.926,47.404,1650,{.forwards=true,.maxSpeed=60,.minSpeed=50},false);
	onMatchLoadReset(true,true);
	chassis.moveToPoint(-21.183,46.404,1500,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	chassis.moveToPoint(-34.183,35.404,500,{.forwards=true,.maxSpeed=120,.minSpeed=50},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-10.101, 35.404, 1000,{.forwards=false,.maxSpeed=60,.minSpeed=50},false);
}

void right9blocks(){
	chassis.setPose(-87.677,-14.462,90);
	scoring(Scoring::HOARD);
	descore(Descore::WINGS_EXTEND);
	chassis.turnToHeading(115, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},true);
	chassis.moveToPoint(-17.5,-25.5,2000,{.maxSpeed=75},false);
	chassis.turnToHeading(140, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},true);
	chassis.moveToPoint(-6.761, -42.175, 2000,{.maxSpeed=60,.minSpeed=20},false);
	matchload(Matchload::EXTEND);
	pros::delay(500);
	chassis.moveToPoint(-19.711,-40.894,2000,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	chassis.moveToPoint(-31.985, -48.404, 2000,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(false,false);
	chassis.moveToPoint(-23.183,-47.404,1300,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-62.926,-47.404,1650,{.forwards=true,.maxSpeed=80,.minSpeed=50},false);
	onMatchLoadReset(false,false);
	chassis.moveToPoint(-21.183,-47.404,1500,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	chassis.moveToPoint(-34.183,-60.404,500,{.forwards=true,.maxSpeed=120,.minSpeed=50},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-10.101, -60.404, 1000,{.forwards=true,.maxSpeed=60,.minSpeed=50},false);
}
