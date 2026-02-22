#include "auton_externs.hpp"
#include "odomreset_fixed.hpp"
#include "robot_states.hpp"
ASSET(leftgetblocksmidgoal_txt);
// Left mid-goal & Right low-goal combined
void leftmidgoalandlong(){
	chassis.setPose(-48.79,16.099,0);
	matchload(Matchload::EXTEND);
	chassis.moveToPoint(-45.842, 45.004,1000,{.forwards=true,.maxSpeed=70,.minSpeed=20},false);
	scoring(Scoring::HOARD);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(true,true);
	chassis.moveToPoint(-64.926,46.404,1050,{.forwards=true,.maxSpeed=90,.minSpeed=20},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(true,true);
	chassis.moveToPoint(-20.183,46.004,1300,{.forwards=false,.maxSpeed=120},false);
	onMatchLoadReset(true,true);
	matchload(Matchload::RETRACT);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1500);
	scoring(Scoring::HOARD);
	chassis.follow(leftgetblocksmidgoal_txt,10,4000,true);
	chassis.moveToPoint(-7.55, 5.6, 1400,{.forwards=false,.maxSpeed=60,.minSpeed=20},false);
	pros::delay(200);
	scoring(Scoring::MID_GOAL);
	pros::delay(1000);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-34.183,32.404,2600,{.forwards=true,.maxSpeed=120,.minSpeed=50},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-15.101, 32.404, 1000,{.forwards=false,.maxSpeed=60,.minSpeed=50},false);
}

void rightlowgoalandlong(){
	chassis.setPose(-48.79,-16.099,180);
	matchload(Matchload::EXTEND);
	chassis.moveToPoint(-45.842, -45.004,1000,{.forwards=true,.maxSpeed=70,.minSpeed=20},false);
	scoring(Scoring::HOARD);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(false,false);
	chassis.moveToPoint(-67.926,-44.404,1250,{.forwards=true,.maxSpeed=120,.minSpeed=20},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(false,false);
	chassis.moveToPoint(-20.183,-44.604,2500,{.forwards=false,.maxSpeed=100,.minSpeed=20},false);
	matchload(Matchload::RETRACT);
	scoring(Scoring::LONG_GOAL);
	pros::delay(1200);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-34.183,-35.404,500,{.forwards=true,.maxSpeed=120,.minSpeed=50},false);
	chassis.moveToPoint(-19, -19, 1000,{.forwards=true,.maxSpeed=60,.minSpeed=20},false);
	pros::delay(300);
	lowgoal.extend();
	chassis.moveToPoint(-13.5, -12,1000,{.forwards=true,.maxSpeed=60,.minSpeed=20},false);
	scoring(Scoring::OUTAKE);
	pros::delay(1500);
	chassis.moveToPoint(-39.183,-34.904,1000,{.forwards=false,.maxSpeed=120,.minSpeed=50},false);
	lowgoal.retract();
	chassis.turnToHeading(90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-13.101, -34.904, 1000,{.forwards=true,.maxSpeed=60,.minSpeed=50},false);
	chassis.turnToHeading(70, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
}
