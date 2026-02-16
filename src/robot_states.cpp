#include "pros/motor_group.hpp"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"
#include <cmath>

#include "robot_states.hpp"
#include "pros/distance.hpp"

extern pros::MotorGroup intake_mg;
extern pros::MotorGroup scoring_mg;
extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;
extern pros::adi::Pneumatics midgoal;
extern pros::adi::Pneumatics matchloader;
extern pros::adi::Pneumatics wings;
extern pros::adi::Pneumatics lowgoal;
extern pros::adi::Pneumatics midgoal_descore;
extern pros::Distance distMatchload;


// Function definitions for actions
inline void setScoreLong() {
    intake_mg.move(127);
    scoring_mg.move(127);
}

inline void setScoreMid() {
    midgoal.extend();
    intake_mg.move(100);
    scoring_mg.move(60);
}
inline void setScoreHoard() {
    intake_mg.move(127);
    scoring_mg.brake();
}
inline void setScoreOutake() {
    intake_mg.move(-127);
}
inline void setScoreLow() {
    lowgoal.extend();
    intake_mg.move(-100);
    
}

inline void setMatchload() {
    matchloader.extend();
    intake_mg.move(127);
}

inline void setToggle() {
		left_mg.move(-20);
		right_mg.move(-20);
		pros::delay(300);
        left_mg.move(40);
		right_mg.move(40);
		pros::delay(500);
		left_mg.brake();
		right_mg.brake();
}
inline void matchloading(){
    // Wait until a ball is detected at the matchload distance sensor
    // If the sensor reads >150 for more than 50ms, exit the function
    uint32_t start_time = pros::millis();
    uint32_t sustained_high_start = 0;

    while (true) {
        // overall safety timeout after 5 seconds
        if (pros::millis() - start_time > 5000) {
            left_mg.brake();
            right_mg.brake();
            return;
        }

        int dist = distMatchload.get_distance();
        if (dist < 0) {
            // sensor error/unset - treat as "no ball" (allow sustained-high logic below)
            dist = 10000;
        }

        if (dist > 150) {
            // start or continue counting how long the sensor has been high
            if (sustained_high_start == 0) sustained_high_start = pros::millis();
            else if (pros::millis() - sustained_high_start > 50) {
                // sensor has been >150 for more than 50ms -> exit
                left_mg.brake();
                right_mg.brake();
                return;
            }
        } else {
            // distance is <=150 -> ball detected, stop movement and continue
            sustained_high_start = 0;
            left_mg.brake();
            right_mg.brake();
            break;
        }

        // small drive forward while checking
        left_mg.move(20);
        right_mg.move(20);
        pros::delay(10);
    }

    pros::delay(500);
    // Ball is detected, continue with matchloading operations
}

inline void setDescoreWings() {
    wings.retract();
}

inline void setDescoreHood() {
    left_mg.move(60);
    right_mg.move(60);
    pros::delay(300);
    left_mg.move(-90);
    right_mg.move(-90);
    pros::delay(500);
}
inline void setDescoreMidgoal() {
    midgoal_descore.extend();
}
inline void matchloadretract() {
    matchloader.retract();
}

inline void scoringsetNone() {
    intake_mg.brake();
    scoring_mg.brake();
    midgoal.retract();
}
inline void wingsextend() {
    wings.extend();
}

static void antijam_check() {
    // Check if intake current is high (indicating a jam)
    if (intake_mg.get_power() < 0.05) {  // Threshold: 2A (adjust as needed)
        // Reverse intake briefly to clear jam
        intake_mg.move(-127);
        pros::delay(50);
        intake_mg.move(127);  // Reverse for 200ms
        // Resume normal operation (caller should set back to desired speed)
    }
}

// Task-compatible wrapper so the function can be started as a PROS Task.
// This loops and periodically calls the helper above.
void antijam(void* /*param*/) {
    while (true) {
        antijam_check();
        pros::delay(50);
    }
}
void scoring(Scoring state) {
    switch (state) {
        case HOARD: setScoreHoard(); break;
        case OUTAKE: setScoreOutake(); break;
        case LONG_GOAL: setScoreLong(); break;
        case MID_GOAL: setScoreMid(); break;
        case LOW_GOAL: setScoreLow(); break;
        case NONE: scoringsetNone(); break;
    }
}

void matchload(Matchload state) {
    switch (state) {
        case EXTEND: setMatchload(); break;
        case MATCHLOADING: matchloading(); break;
        case RETRACT: matchloader.retract(); break;
    }
}

void descore(Descore state) {
    switch (state) {
        case WINGS_EXTEND: wingsextend(); break;
        case HOOD: setDescoreHood(); break;
        case MIDGOAL_DESCORE: setDescoreMidgoal(); break;
        case WINGS_RETRACT: wings.retract(); break;
    }
}

