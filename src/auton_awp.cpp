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
	chassis.moveToPoint(-48.447,-46.004,900,{.forwards=true,.maxSpeed=1270},false);
	chassis.turnToHeading(270, 600,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=127,.minSpeed=20});
	resetposition(true, false,false);
	chassis.moveToPoint(-63.926,-46.904,1200,{.forwards=true,.maxSpeed=127,.minSpeed=10});
	chassis.moveToPoint(-21.183,-46.404,1200,{.forwards=false,.maxSpeed=127,.minSpeed=10},false);
	resetposition(true, false,false);
	matchload(Matchload::RETRACT);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-47.183,-47.404,500,{.forwards=true,.maxSpeed=127,.minSpeed=20},false);
	chassis.moveToPoint(-22, -17, 1050,{.forwards=true,.maxSpeed=127,.minSpeed=20},false);
	matchload(Matchload::EXTEND);
	chassis.turnToHeading(0,600,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=127,.minSpeed=20},false);	
	matchload(Matchload::RETRACT);
	chassis.moveToPoint(-28, 22, 1200,{.forwards=true,.maxSpeed=127,.minSpeed=20},false);
	matchload(Matchload::EXTEND);
	chassis.moveToPoint(-8.55, 10.6, 1000,{.forwards=false,.maxSpeed=127},false);
	scoring(Scoring::MID_GOAL);
	pros::delay(600);
	scoring(Scoring::OUTAKE);
	pros::delay(100);
	midgoal.retract();
	scoring(Scoring::HOARD);
	matchload(Matchload::RETRACT);
	chassis.moveToPoint(-50.447,47.004,1200,{.maxSpeed=127,.minSpeed=20},false);
	chassis.turnToHeading(270, 600,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=127,.minSpeed=20},true);
	resetposition(false,true,true);
	matchload(Matchload::EXTEND);
	chassis.moveToPoint(-65.926,47.004,1200,{.forwards=true,.maxSpeed=127,.minSpeed=20},false);
	resetposition(false,true,true);
	chassis.moveToPoint(-21.183,46.404,1000,{.forwards=false,.maxSpeed=127,.minSpeed=20},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	resetposition(false,true,true);
}
