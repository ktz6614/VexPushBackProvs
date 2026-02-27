#include "auton_externs.hpp"
#include "odomreset_fixed.hpp"
#include "pros/distance.hpp"
#include "pros/rtos.hpp"
#include "robot_states.hpp"
#include <cstdint>

extern pros::Distance distMatchload;
extern pros::Distance distBack;
bool ballthere=false;
bool task_running = true;

void setToggle(){
	chassis.tank(-20,-20);
	pros::delay(300);
    chassis.tank(40,40);
	pros::delay(350);
	chassis.cancelAllMotions();

}
void loadloader(double x_point,double y_point){
    chassis.moveToPoint(x_point,y_point,2000,{.maxSpeed=127},true);
	int loop=0;
	pros::delay(500);
    if(distMatchload.get_distance() > 150){
        while(distMatchload.get_distance()>150&& loop<5){
		setToggle();
		loop++;} 

    }
	chassis.tank(10,10);
	pros::delay(500);

	int timeout = 0;
	int counter =0;
    while (timeout < 80 && counter < 30) {
        timeout++;
        if (distMatchload.get_distance() >= 150) {
            counter++;
        } else {
            counter = 0;
        }
        pros::delay(20);
    }  
}
void loadloader1(double x_point,double y_point){
    chassis.moveToPoint(x_point,y_point,2000,{.maxSpeed=127},true);
	int loop=0;
	pros::delay(500);
    if(distMatchload.get_distance() > 150){
        while(distMatchload.get_distance()>150&& loop<5){
		setToggle();
		loop++;} 

    }
	chassis.tank(10,10);
	pros::delay(500);
	chassis.cancelAllMotions();
	int timeout = 0;
	int counter =0;
    while (timeout < 80 && counter < 2) {
        timeout++;
		setToggle();
        if (distMatchload.get_distance() >= 150) {
            counter++;
        } else {
            counter = 0;
        }
        pros::delay(20);
    }  
}
void park(){
	while(distBack.get_distance()<1600){
		chassis.tank(50,50);
		master.print(0,0,"%.2f",distBack.get_distance());
	}
	chassis.cancelAllMotions();
}
void scoreskills(double x_point,double y_point){
    chassis.moveToPoint(x_point,y_point,1000,{.maxSpeed=80});
    while(left_mg.get_actual_velocity()>5 && right_mg.get_actual_velocity()>5 ){
        pros::delay(20);
    }
	chassis.cancelAllMotions();
	chassis.tank(-15,-15);
	if(scoring_mg.get_power()<9 && scoring_mg.get_power()>3){
		scoring_mg.move(-127);
		scoring(Scoring::OUTAKE);
		pros::delay(100);
		scoring(Scoring::LONG_GOAL);
	}
	scoring(Scoring::LONG_GOAL);
	pros::delay(2300);
	chassis.cancelAllMotions();
	scoring(Scoring::HOARD);
}
void skills(){
	pros::Task antijam_task(antijam);
	chassis.setPose(-48.79,-16.099,180);
	matchload(Matchload::EXTEND);
	descore(WINGS_EXTEND);
	chassis.moveToPoint(-48.842, -46.004,1000,{.forwards=true,.maxSpeed=70,.minSpeed=20},false);
	scoring(Scoring::HOARD);
	chassis.turnToHeading(270, 1000,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(true, false,false);
	loadloader(-62.926, -46.504);
	chassis.moveToPoint(-45.183,-62.004,2500,{.forwards=false,.maxSpeed=100,.minSpeed=20},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(true, false,false);
	chassis.moveToPoint(40.183,-62.004,2500,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.moveToPoint(45.183,-46.504,2000,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.turnToHeading(90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(false,false,false);
	chassis.moveToPoint(20.183,-46.904,1300,{.forwards=false,.maxSpeed=80,.minSpeed=20},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(2300);
	scoring(Scoring::HOARD);
	matchload(Matchload::EXTEND);
	resetposition(false,false,false);
	loadloader(62.826,-46.904);
	chassis.moveToPoint(20.183,-46.004,1300,{.forwards=false,.maxSpeed=80,.minSpeed=20},false);
	matchload(Matchload::RETRACT);
	resetposition(false,false,false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(2300);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(38.183,-47.504,1000,{.forwards=true,.maxSpeed=80},false);
	resetposition(false,false,false);
	chassis.turnToHeading(0, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(38.183,45.504,5000,{.forwards=true,.maxSpeed=80,.minSpeed=20},false);
	chassis.turnToHeading(90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	matchload(Matchload::EXTEND);
	resetposition(true,true,true);
	loadloader(63.826,46.604);
	chassis.moveToPoint(40.183,65.004,2000,{.forwards=false,.maxSpeed=100},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(true,true,true);
	chassis.moveToPoint(-40.183,62.004,2500,{.forwards=false,.maxSpeed=120,.minSpeed=5},false);
	chassis.moveToPoint(-45.183,46.504,2000,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(false,true,true);
	chassis.moveToPoint(-20.183,45.904,1300,{.forwards=false,.maxSpeed=80,.minSpeed=20},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(2300);
	scoring(Scoring::HOARD);
	matchload(Matchload::EXTEND);
	resetposition(false,true,true);
	loadloader(-61.026,46.904);
	matchload(Matchload::MATCHLOADING);
	chassis.moveToPoint(-20.183,46.004,1300,{.forwards=false,.maxSpeed=120},false);
	scoring(Scoring::LONG_GOAL);
	pros::delay(2300);
	matchload(Matchload::RETRACT);
	scoring(Scoring::HOARD);
	resetposition(false,true,true);
	descore(WINGS_EXTEND);
	chassis.moveToPoint(-38.183,46.004,500,{.forwards=true,.maxSpeed=120},false);
	chassis.turnToHeading(180, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-38.183,0,1300,{.forwards=true,.maxSpeed=120},false);
	chassis.turnToHeading(270,800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-70, 0, 1000,{.forwards=true,.maxSpeed=120,.minSpeed=100},false);
	chassis.tank(100,100);
}
