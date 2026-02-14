#ifndef ROBOT_STATES_HPP
#define ROBOT_STATES_HPP
enum Scoring {
    HOARD,
    OUTAKE,
    LONG_GOAL,
    MID_GOAL,
    LOW_GOAL,
    NONE
};

enum Matchload {
    EXTEND,
    MATCHLOADING,
    RETRACT
};

enum Descore {
    WINGS_EXTEND,
    HOOD,
    MIDGOAL_DESCORE,
    WINGS_RETRACT
};
void scoring(Scoring state);

void matchload(Matchload state);

void descore(Descore state);

// Task wrapper declaration for the antijam background task (takes a void* per PROS Task API)
void antijam(void*);
void setToggle();

#endif // ROBOT_STATES_HPP