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
	descore(Descore::WINGS_EXTEND);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-48.447,-46.004,900,{.forwards=true,.maxSpeed=120},false);
	chassis.turnToHeading(270, 600,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(true, false,false);
	chassis.moveToPoint(-63.926,-46.904,1000,{.forwards=true,.maxSpeed=120},false);
	chassis.moveToPoint(-21.183,-46.304,1400,{.forwards=false,.maxSpeed=90},false);
	resetposition(true, false,false);
	matchload(Matchload::RETRACT);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-30.183,-30.404,500,{.forwards=true,.maxSpeed=120},false);
	chassis.moveToPoint(-24, -22, 650,{.forwards=true,.maxSpeed=120},false);
	matchload(Matchload::EXTEND);
	chassis.turnToHeading(0,600,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=120},false);	
	matchload(Matchload::RETRACT);
	chassis.moveToPoint(-28, 22, 1000,{.forwards=true,.maxSpeed=120},false);
	matchload(Matchload::EXTEND);
	chassis.moveToPoint(-9.55, 5.6, 1000,{.forwards=false,.maxSpeed=120},false);
	scoring(Scoring::MID_GOAL);
	pros::delay(600);
	scoring(Scoring::OUTAKE);
	pros::delay(100);
	midgoal.retract();
	scoring(Scoring::HOARD);
	matchload(Matchload::RETRACT);
	chassis.moveToPoint(-55.447,45.004,1200,{.maxSpeed=120},false);
	chassis.turnToHeading(270, 700,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},true);
	resetposition(false,true,true);
	matchload(Matchload::EXTEND);
	chassis.moveToPoint(-63.926,45.004,1000,{.forwards=true,.maxSpeed=120},false);
	resetposition(false,true,true);
	chassis.moveToPoint(-21.183,45.904,1000,{.forwards=false,.maxSpeed=120},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	resetposition(false,true,true);
}
