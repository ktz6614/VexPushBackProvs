#pragma once
// Which wall (if any) is being touched/read by the distance sensors
enum Wall { None = 0, Left, Right };

// Convert mm → inches
double mmToInches(double mm);

// Check if reading is *very close*, meaning likely flush against wall
bool isTouchingWall(double readingInches, double threshold = 25.0);

// Determine which wall is being touched (left has priority)
Wall detectTouchedWall(bool leftPositive, bool rightPositive);

// Reset helper: set ±magnitude for left/right walls, do NOT change theta
// - leftPositive/rightPositive: choose +magnitude or -magnitude for that wall
// - magnitude: absolute coordinate to set (default 48.0)
// - applyToX: when true set X coordinate, otherwise set Y coordinate
void resetPoseLeftRightOption(bool leftPositive, bool rightPositive, double magnitude = 72, bool applyToX = true);

double headingErrorDeg(double heading, double target);
double correctedWallDistanceInches(double raw_mm, double heading, double targetHeading);

// Convenience helpers for match-load resets
void onMatchLoadReset_fixed(bool leftPositive, bool rightPositive, bool force = false);
void onMatchLoadReset(bool leftPositive, bool rightPositive);

void resetPoseWithCosineCorrection(bool leftwall, bool leftPositive, bool rightPositive, bool force = false);

void resetposition(bool leftwall, bool leftPositive, bool rightPositive);