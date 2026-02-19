#include "main.h"
#include <cmath>
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "pros/screen.h"
#include "odomreset_fixed.hpp"
#include "robot_states.hpp"
#include "auton_9blocks.hpp"
#include "auton_7blocks.hpp"
#include "auton_mid_low.hpp"
#include "auton_awp.hpp"
#include "skills.hpp"
#include "lemlib.hpp"
#include "odomreset_fixed.hpp"
#define ENABLE_SKILLS_MACRO 0
#define ENABLE_AUTON_SELCTOR 0
#define TEST 1
enum Auton {
    SKILLS,
    LEFT_9BLOCKS,
    LEFT_7BLOCKS,
    LEFT_MIDGOAL_AND_LONG,
    LEFT_SNACKYAHH,
    RIGHT_9BLOCKS,
    RIGHT_LOWGOAL_AND_LONG,
    RIGHT_7BLOCKS,
    RIGHT_SAW_P
};

Auton selected_auton = SKILLS;

enum Screen { MAIN, SIDE, PROGRAM };

Screen current_screen = MAIN;

struct Button {
    int x1, y1, x2, y2;
    const char* text;
    Auton auton; // for program buttons
};

Button main_buttons[2] = {
    {50, 50, 200, 100, "Game Auton", (Auton)-1},
    {250, 50, 400, 100, "Skills", SKILLS}
};

Button side_buttons[2] = {
    {50, 50, 200, 100, "Left", (Auton)-1},
    {250, 50, 400, 100, "Right", (Auton)-1}
};

Button left_program_buttons[4] = {
    {50, 20, 350, 60, "9 Blocks", LEFT_9BLOCKS},
    {50, 70, 350, 110, "7 Blocks", LEFT_7BLOCKS},
    {50, 120, 350, 160, "Midgoal & Long", LEFT_MIDGOAL_AND_LONG},
    {50, 170, 350, 210, "Snacky Hard Carry", LEFT_SNACKYAHH}
};

Button right_program_buttons[4] = {
    {50, 20, 350, 60, "9 Blocks", RIGHT_9BLOCKS},
    {50, 70, 350, 110, "Lowgoal & Long", RIGHT_LOWGOAL_AND_LONG},
    {50, 120, 350, 160, "7 Blocks", RIGHT_7BLOCKS},
    {50, 170, 350, 210, "Solo AWP", RIGHT_SAW_P}
};

bool left_selected = false;


void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup intake_mg({7});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
pros::MotorGroup scoring_mg({8});
pros::MotorGroup left_mg({-13, -2,-14});  //3 Creates a motor group with forwards ports 4 & 5
pros::MotorGroup right_mg({15, 16,19}); //21 Creates a motor group with
pros::adi::Pneumatics midgoal('H', false);
pros::adi::Pneumatics matchloader('D',false);
pros::adi::Pneumatics wings('G',false);
pros::adi::Pneumatics lowgoal('F',false);
pros::adi::Pneumatics midgoal_descore('E',false);
pros::Rotation vertical_encoder(-12);
pros::Imu imu(11);

// Distance sensor definitions (declared extern in odomreset.hpp)
pros::Distance distLeft(10);
pros::Distance distRight(9);
pros::Distance distMatchload(18);

void initialize() {
	pros::lcd::initialize();
	chassis.calibrate();
}
void displayposition(){

	while(true){
		pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta);
		pros::delay(50);
	}
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
void competition_initialize() {
#if ENABLE_AUTON_SELCTOR
   while (true) {
        // Draw buttons based on current_screen
        if (current_screen == MAIN) {
            for (auto &btn : main_buttons) {
                pros::screen::set_pen(pros::c::COLOR_BLUE);
                pros::screen::fill_rect(btn.x1, btn.y1, btn.x2, btn.y2);
                pros::screen::set_pen(pros::c::COLOR_WHITE);
                pros::screen::draw_rect(btn.x1, btn.y1, btn.x2, btn.y2);
                pros::screen::print(TEXT_MEDIUM, btn.x1 + 10, btn.y1 + 20, btn.text);
            }
        } else if (current_screen == SIDE) {
            for (auto &btn : side_buttons) {
                pros::screen::set_pen(pros::c::COLOR_BLUE);
                pros::screen::fill_rect(btn.x1, btn.y1, btn.x2, btn.y2);
                pros::screen::set_pen(pros::c::COLOR_WHITE);
                pros::screen::draw_rect(btn.x1, btn.y1, btn.x2, btn.y2);
                pros::screen::print(TEXT_MEDIUM, btn.x1 + 10, btn.y1 + 20, btn.text);
            }
        } else if (current_screen == PROGRAM) {
            Button *buttons = left_selected ? left_program_buttons : right_program_buttons;
            for (int i = 0; i < 4; i++) {
                auto &btn = buttons[i];
                pros::screen::set_pen(pros::c::COLOR_BLUE);
                pros::screen::fill_rect(btn.x1, btn.y1, btn.x2, btn.y2);
                pros::screen::set_pen(pros::c::COLOR_WHITE);
                pros::screen::draw_rect(btn.x1, btn.y1, btn.x2, btn.y2);
                pros::screen::print(TEXT_MEDIUM, btn.x1 + 10, btn.y1 + 10, btn.text);
            }
        }

        // Check touch
        pros::screen_touch_status_s_t status = pros::screen::touch_status();
        if (status.touch_status == TOUCH_PRESSED || status.touch_status == TOUCH_HELD) {
            int x = status.x;
            int y = status.y;
            if (current_screen == MAIN) {
                for (auto &btn : main_buttons) {
                    if (x >= btn.x1 && x <= btn.x2 && y >= btn.y1 && y <= btn.y2) {
                        if (strcmp(btn.text, "Skills") == 0) {
                            selected_auton = SKILLS;
                            // Show selected
                            pros::screen::set_pen(pros::c::COLOR_BLACK);
                            pros::screen::fill_rect(0, 0, 480, 240);
                            pros::screen::set_pen(pros::c::COLOR_WHITE);
                            pros::screen::print(TEXT_MEDIUM, 100, 100, "Skills");
                            pros::delay(1000);
                            return; // exit
                        } else {
                            current_screen = SIDE;
                        }
                        pros::delay(200); // debounce
                        break;
                    }
                }
            } else if (current_screen == SIDE) {
                for (auto &btn : side_buttons) {
                    if (x >= btn.x1 && x <= btn.x2 && y >= btn.y1 && y <= btn.y2) {
                        left_selected = (strcmp(btn.text, "Left") == 0);
                        current_screen = PROGRAM;
                        pros::delay(200);
                        break;
                    }
                }
            } else if (current_screen == PROGRAM) {
                Button *buttons = left_selected ? left_program_buttons : right_program_buttons;
                for (int i = 0; i < 4; i++) {
                    auto &btn = buttons[i];
                    if (x >= btn.x1 && x <= btn.x2 && y >= btn.y1 && y <= btn.y2) {
                        selected_auton = btn.auton;
                        // Show selected
                        pros::screen::set_pen(pros::c::COLOR_BLACK);
                        pros::screen::fill_rect(0, 0, 480, 240);
                        pros::screen::set_pen(pros::c::COLOR_WHITE);
                        const char* side = left_selected ? "Left " : "Right ";
                        pros::screen::print(TEXT_MEDIUM, 100, 100, "%s%s", side, btn.text);
                        pros::delay(1000);
                        return; // exit
                    }
                }
            }
        }

        pros::delay(50);
    }
	#endif
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void parkingzone(){
	chassis.setPose(-46.832,0,270);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-60.832,-0,1500,{.forwards=true,.maxSpeed=120,.minSpeed=20},false);
	pros::delay(500);
	chassis.moveToPoint(-68.832,-0,1000,{.forwards=true,.maxSpeed=120,.minSpeed=20},false);
	chassis.turnToHeading(269, 1200);
	chassis.turnToHeading(289, 1000);
	chassis.moveToPoint(-60.832,-0,1500,{.forwards=true,.maxSpeed=120,.minSpeed=20},false);
	pros::delay(500);}
// skills moved to include/auton_skills.hpp

void autonomous() {
	left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	pros::Task displaytask(displayposition);
	#if TEST
	skills();
	#endif
 	#if ENABLE_AUTON_SELCTOR
	switch(selected_auton){
        case SKILLS: skills(); break;
        case LEFT_9BLOCKS: left9blocks(); break;
        case LEFT_7BLOCKS: left7blocks(); break;
        case LEFT_MIDGOAL_AND_LONG: leftmidgoalandlong(); break;
        case LEFT_SNACKYAHH: leftsnackyahh(); break;
        case RIGHT_9BLOCKS: right9blocks(); break;
        case RIGHT_LOWGOAL_AND_LONG: rightlowgoalandlong(); break;
        case RIGHT_7BLOCKS: right7blocks(); break;
        case RIGHT_SAW_P: rightsawp(); break;
    }
	#endif
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
// Skills Macro System
#if ENABLE_SKILLS_MACRO
enum SkillsStep {
	SKILLS_IDLE,
	SKILLS_STEP_2,
	SKILLS_STEP_3,
	SKILLS_STEP_4,
	SKILLS_STEP_5,
	SKILLS_COMPLETE
};

SkillsStep current_skills_step = SKILLS_IDLE;
bool prevRightArrow = false;
void clear_parkingzone(){
	chassis.setPose(-46.832,0,270);
	master.rumble("..");
}void gotomidgoal(){
	chassis.turnToHeading(315,800,{.maxSpeed=100},false);
	chassis.moveToPoint(-7.55, 5.6, 1000,{.forwards=false,.maxSpeed=60,.minSpeed=20},false);
	chassis.turnToHeading(315,800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	pros::delay(200);
	master.rumble("..");
}
void gotoloader1(){
	chassis.setPose(-8.55,10.6,315);
	chassis.moveToPoint(-38.404,42.404,2300,{.forwards=true,.maxSpeed=80,.minSpeed=50},false);
	chassis.turnToHeading(270, 600,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(false,true,true);
	matchload(Matchload::EXTEND);
	master.rumble("..");

}
void gotoscoreloader1(){
	scoring(Scoring::HOARD);
	chassis.moveToPoint(-45.183,62.004,2000,{.forwards=false,.maxSpeed=120,.minSpeed=60},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(false,true,true);
	chassis.moveToPoint(42.183,62.004,2500,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.moveToPoint(42.183,46.504,2000,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.turnToHeading(90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(true,true,true);
	chassis.moveToPoint(20.183,46.404,1300,{.forwards=false,.maxSpeed=80,.minSpeed=20},false);
	resetposition(true,true,true);
	master.rumble("..");
}
void gotoparkingzone(){
	descore(Descore::WINGS_EXTEND);
	scoring(Scoring::HOARD);
	chassis.moveToPoint(63,26.373,1200,{.forwards=true,.maxSpeed=80,.minSpeed=20},false);
	chassis.turnToHeading(180, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(63,10,500,{.forwards=true,.maxSpeed=80,.minSpeed=20},false);
	chassis.cancelAllMotions();
	pros::delay(500);
	chassis.tank(50,50);
	pros::delay(3500);
	chassis.cancelAllMotions();
	master.rumble("..");

}
void goscoreloader3(){
	scoring(Scoring::HOARD);
	resetposition(false,false,false);
	chassis.moveToPoint(45.183,-65.004,2500,{.forwards=false,.maxSpeed=120,.minSpeed=20},false);
	resetposition(false,false,false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(-42.183,-65.004,2500,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.moveToPoint(-42.183,-53.504,2500,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	resetposition(true,false,false);
	chassis.moveToPoint(-20.183,-53.904,1300,{.forwards=false,.maxSpeed=80,.minSpeed=20},false);
	resetposition(true,false,false);
	master.rumble("..");

}

void skills_step_2() {
	onMatchLoadReset(false,false);
	chassis.moveToPoint(-45.183,-60.004,2500,{.forwards=false,.maxSpeed=100,.minSpeed=20},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(40.183,-60.004,2500,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.moveToPoint(45.183,-46.504,2500,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.turnToHeading(90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(false,false);
	chassis.moveToPoint(20.183,-46.904,1300,{.forwards=false,.maxSpeed=80,.minSpeed=20},false);
	onMatchLoadReset(false,false);
	// TODO: Implement your second skill step here
}

void skills_step_3() {
	// TODO: Implement your third skill step here
	chassis.moveToPoint(38.183,-47.504,1000,{.forwards=true,.maxSpeed=60},false);
	onMatchLoadReset(false,false);
	chassis.turnToHeading(0, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	chassis.moveToPoint(38.183,47.504,5000,{.forwards=true,.maxSpeed=80,.minSpeed=20},false);
	chassis.turnToHeading(90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	matchload(Matchload::EXTEND);
	onMatchLoadReset(true,true);
}

void skills_step_4() {
	// TODO: Implement your fourth skill step here
	chassis.moveToPoint(45.183,62.004,3000,{.forwards=false,.maxSpeed=100,.minSpeed=20},false);
	matchload(Matchload::RETRACT);
	chassis.turnToHeading( 90, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(true,true);
	chassis.moveToPoint(-40.183,62.004,2500,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.moveToPoint(-45.183,46.504,2500,{.forwards=false,.maxSpeed=100,.minSpeed=5},false);
	chassis.turnToHeading(270, 800,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
	onMatchLoadReset(true,true);
	chassis.moveToPoint(-20.183,46.504,1300,{.forwards=false,.maxSpeed=80,.minSpeed=20},false);
}


void execute_skills_macro() {
	switch(current_skills_step) {
		case SKILLS_STEP_2:
			gotoloader1();
			break;
		case SKILLS_STEP_3:
			gotoscoreloader1();
			break;
		case SKILLS_STEP_4:
			gotoparkingzone();
			break;
		case SKILLS_STEP_5:
			goscoreloader3();
			break;
		case SKILLS_COMPLETE:
			break;
		default:
			break;
	}
}
#endif

void opcontrol() {
	bool prevY = false;
	bool prevA = false;
	bool prevB = false;
	bool prevX = false;
	left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs
#if ENABLE_SKILLS_MACRO
		// --- Right Arrow (Controller Right Button - Skills Macro Advance) ---
		bool rightArrowPressed = master.get_digital(DIGITAL_RIGHT);
		if (rightArrowPressed && !prevRightArrow && selected_auton == SKILLS) {
			if (current_skills_step == SKILLS_IDLE) {
				current_skills_step = SKILLS_STEP_2;
				execute_skills_macro();
			} else if (current_skills_step == SKILLS_STEP_2) {
				current_skills_step = SKILLS_STEP_3;
				execute_skills_macro();
			} else if (current_skills_step == SKILLS_STEP_3) {
				current_skills_step = SKILLS_STEP_4;
				execute_skills_macro();
			} else if (current_skills_step == SKILLS_STEP_4) {
				current_skills_step = SKILLS_STEP_5;
				execute_skills_macro();
			} else if (current_skills_step == SKILLS_STEP_5) {
				current_skills_step = SKILLS_COMPLETE;
				execute_skills_macro();
			}
		}
		prevRightArrow = rightArrowPressed;
#endif
		
		// Arcade control scheme
		int dir = 0.95*master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = 0.5*master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir + turn);                      // Sets left motor voltage
		right_mg.move(dir - turn);                     // Sets right motor voltage
		if (master.get_digital(DIGITAL_R1)) {
			scoring(Scoring::HOARD);
		} else if (master.get_digital(DIGITAL_R2)) {
			scoring(Scoring::OUTAKE);
		} 
		else if (master.get_digital(DIGITAL_L1)) {
			scoring(Scoring::LONG_GOAL);
		} else if (master.get_digital(DIGITAL_L2)) {
			scoring(Scoring::MID_GOAL);

		}else if(master.get_digital(DIGITAL_UP)){
			chassis.setPose(-21.183,46.004,270);
			chassis.moveToPoint(-31.183,34.404,800,{.forwards=true,.maxSpeed=120},false);
			chassis.turnToHeading(270, 300,{.direction=lemlib::AngularDirection::AUTO,.maxSpeed=100},false);
			chassis.moveToPoint(-10.101, 35.404, 500,{.forwards=false,.maxSpeed=60},false);
		} else {
			scoring(Scoring::NONE);
		}
		bool xPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_X);
		if (xPressed && !prevX) {
			if(midgoal_descore.is_extended()== false){
				midgoal_descore.extend();
			}else{
				midgoal_descore.retract();
			}
		}
		prevX= xPressed;
				// --- X Button (Matchload Extend/Retract Toggle) ---
		bool yPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_Y);
		if (yPressed && !prevY) {
			// Toggle matchloader: extend if retracted, retract if extended
			if (matchloader.is_extended() == false) { // if retracted
				matchload(Matchload::EXTEND);
			} else { // if extended
				matchload(Matchload::RETRACT);
			}
		}
		prevY= yPressed;

		// --- Y Button (Hoard/Midgoal Toggle) ---

		bool aPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		if (aPressed && !prevA) {
			// Simple toggle - extend if retracted, retract if extended
			if (wings.is_extended() == false) { // if retracted
				descore(Descore::WINGS_EXTEND);
// Feedback
			} else { // if extended
				descore(Descore::WINGS_RETRACT);
			}
		}
		prevA = aPressed;
		bool bPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
		if (bPressed && !prevB) {
			// Simple toggle - extend if retracted, retract if extended
			if (lowgoal.is_extended() == false) { // if retracted
				lowgoal.extend();
// Feedback
			} else { // if extended
				lowgoal.retract();
			}
		}
		prevB = bPressed;
		pros::delay(20);                               // Run for 20 ms then update
	}
}