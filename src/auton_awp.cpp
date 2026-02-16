#include "auton_externs.hpp"
#include "odomreset_fixed.hpp"
#include "robot_states.hpp"
// Miscellaneous autonomous routines (sawp, snacky)
void leftsnackyahh(){
	chassis.setPose(-48.79,16.099,0);
	chassis.moveToPoint(-48.842, 20.004,500,{.forwards=true,.maxSpeed=70,.minSpeed=20},false);
}

void rightsawp(){
	chassis.setPose(-48.79,-16.099,180);
	matchload(Matchload::EXTEND);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-46.447,-45.004,1000,{.forwards=true,.maxSpeed=80},false);
	chassis.turnToHeading(270, 600,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(false,false);
	chassis.moveToPoint(-63.926,-46.904,1200,{.forwards=true,.maxSpeed=100},false);
	onMatchLoadReset(false,false);
	chassis.moveToPoint(-21.183,-46.004,1500,{.forwards=false,.maxSpeed=90},false);
	onMatchLoadReset(false,false);
	matchload(Matchload::RETRACT);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-30.183,-30.404,500,{.forwards=true,.maxSpeed=120},false);
	chassis.moveToPoint(-24, -22, 650,{.forwards=true,.maxSpeed=80},false);
	matchload(Matchload::EXTEND);
	chassis.turnToHeading(0,600,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);	
	matchload(Matchload::RETRACT);
	chassis.moveToPoint(-26, 22, 1000,{.forwards=true,.maxSpeed=90},false);
	matchload(Matchload::EXTEND);
	chassis.moveToPoint(-7.55, 5.6, 1000,{.forwards=false,.maxSpeed=90,.minSpeed=20},false);
	chassis.turnToHeading(315, 600);
	scoring(Scoring::MID_GOAL);
	pros::delay(600);
	scoring(Scoring::OUTAKE);
	pros::delay(100);
	midgoal.retract();
	scoring(Scoring::HOARD);
	matchload(Matchload::RETRACT);
	chassis.moveToPoint(-46.447,47.404,1000,{.maxSpeed=120,.minSpeed=10},false);
	chassis.turnToHeading(270, 700,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},true);
	onMatchLoadReset(true,true);
	chassis.moveToPoint(-21.183,46.904,1000,{.forwards=false,.maxSpeed=120},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	onMatchLoadReset(true,true);
	chassis.moveToPoint(-34.183,35.404,700,{.forwards=true,.maxSpeed=120,.minSpeed=20},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-10.101, 35.404, 1000,{.forwards=false,.maxSpeed=60},false);
}
