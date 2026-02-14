#include <cmath>
#include "lemlib/chassis/chassis.hpp"
#include "pros/distance.hpp"
#include "pros/adi.h"
#include "odomreset_fixed.hpp"

// ---- Sensor declarations (defined in main.cpp) ----
// e.g. distFront(15)
extern pros::Distance distBack;
extern pros::Distance distLeft;
extern pros::Distance distRight;
extern pros::Controller master;

// ---- Offsets / Field ----
inline constexpr double OFFSET_LEFT  = 5.0;
inline constexpr double OFFSET_RIGHT = 5.0;
inline constexpr double FIELD_SIZE = 144.0;

extern lemlib::Chassis chassis;

// Convert mm → inches
double mmToInches(double mm) { return mm / 25.4; }

// Check if reading is *very close*, meaning likely flush against wall
bool isTouchingWall(double readingInches, double threshold) {
  return readingInches < threshold;
}

// Determine which wall is being touched (left has priority)
Wall detectTouchedWall(bool leftPositive, bool rightPositive) {
  double l = mmToInches(distLeft.get());
  double r = mmToInches(distRight.get());
  if (isTouchingWall(l)) return Wall::Left;
  if (isTouchingWall(r)) return Wall::Right;
  return Wall::None;
}

Wall chooseWall(bool leftwall, bool leftpositve, bool rightPositive){
  if (leftwall && leftpositve) {
    return Wall::Left;
  } else if (!leftwall && !rightPositive) {
    return Wall::Right;
  } else if (leftwall && !leftpositve) {
    return Wall::Left;
  } else if (!leftwall && rightPositive) {
    return Wall::Right;
  } else {
    return Wall::None;
  }
}
// Reset helper: set ±magnitude for left/right walls, do NOT change theta
// - leftPositive/rightPositive: choose +magnitude or -magnitude for that wall
// - magnitude: absolute coordinate to set (default 48.0)
// - applyToX: when true set X coordinate, otherwise set Y coordinate
void resetPoseLeftRightOption(bool leftPositive,
                                     bool rightPositive,
                                     double magnitude,
                                     bool applyToX) {
  Wall wall = detectTouchedWall(leftPositive,rightPositive);
  if (wall == Wall::None) return;

  auto pose = chassis.getPose();
  double newX = pose.x;
  double newY = pose.y;

  if (wall == Wall::Left) {
    double v = leftPositive ? magnitude : -magnitude;
    if (applyToX) newX = v; else newY = v;
  } else if (wall == Wall::Right) {
    double v = rightPositive ? magnitude : -magnitude;
    if (applyToX) newX = v; else newY = v;
  }

  // Do not change theta (pose.theta)
  chassis.setPose(newX, newY, pose.theta);
}
double headingErrorDeg(double heading, double target) {
  double diff = std::fmod(heading - target + 540.0, 360.0) - 180.0;
  double abs_diff = std::abs(diff);
#ifdef ODOMRESET_DEBUG
  pros::lcd::print(3, "headingDiff: %.2f abs: %.2f", diff, abs_diff);
#endif
  return abs_diff;

} 
double correctedWallDistanceInches(double raw_mm,
                                          double heading,
                                          double targetHeading) {
  double d = raw_mm / 25.4;   // AE
  double thetaDeg = headingErrorDeg(heading, targetHeading);
  double thetaRad = thetaDeg * M_PI / 180.0;
  return d * std::cos(thetaRad); // AD
  
}


// Convenience: default match-load reset that only uses left/right measured distances
// Keeps theta unchanged and only adjusts the Y coordinate (preserves prior behavior)
void onMatchLoadReset_fixed(bool leftPositive, bool rightPositive, bool force) {
  // Measure the touched wall distance, add offsets, then set the
  // corresponding coordinate. The `leftPositive`/`rightPositive` flags
  // control whether the computed coordinate is used as positive or
  // negated. If `force` is true, the function will attempt a reset even
  // if no wall is detected (useful for testing).
  pros::delay(150);
  Wall wall = detectTouchedWall(leftPositive, rightPositive);
  // If no wall detected and not forced, do nothing
  if (wall == Wall::None && !force) {
    // Optionally show debug info
#ifdef ODOMRESET_DEBUG
    pros::lcd::print(3, "Detect: L=%.2f in, R=%.2f in -> None", mmToInches(distLeft.get()), mmToInches(distRight.get()));
#endif
    pros::delay(50);
    return;
  }

  if (wall == Wall::Left || force) {
    auto pose = chassis.getPose();
    double l = distLeft.get(); // mm
    double l_in = l / 25.4; // inches
    double targetY;
    if (leftPositive) {
      targetY = 70.5 - OFFSET_LEFT - l_in;
    } else {
      targetY = -70.5 + OFFSET_LEFT + l_in;
    }
#ifdef ODOMRESET_DEBUG
    pros::lcd::print(3, "Left: raw=%.2fmm (%.2fin) offset=%.2f targetY=%.2f", l, l_in, OFFSET_LEFT, targetY);
#endif
    chassis.setPose(pose.x, targetY, pose.theta);
    pros::delay(50);
    return;
  }

  if (wall == Wall::Right) {
    auto pose = chassis.getPose();
    double r = distRight.get(); // mm
    double r_in = r / 25.4; // inches
    double targetY;
    if (rightPositive) {
      targetY = 70.5 - OFFSET_RIGHT - r_in;
    } else {
      targetY = -70.5 + OFFSET_RIGHT + r_in;
    }
#ifdef ODOMRESET_DEBUG
    pros::lcd::print(3, "Right: raw=%.2fmm (%.2fin) offset=%.2f targetY=%.2f", r, r_in, OFFSET_RIGHT, targetY);
#endif
    chassis.setPose(pose.x, targetY, pose.theta);
  }
  pros::delay(50);
}

// Historical two-arg name (keeps compatibility)
void onMatchLoadReset(bool leftPositive, bool rightPositive) {
  onMatchLoadReset_fixed(leftPositive, rightPositive, false);
}

// Zero-argument convenience overload: default to positive coordinates
void resetPoseWithCosineCorrection(
  bool leftwall,bool leftPositive,
  bool rightPositive,
  bool force
) {
  pros::delay(150);

  auto pose = chassis.getPose();
  double heading = pose.theta;

  // Only allow Y reset near 90° or 270°
  bool near90  = headingErrorDeg(heading, 90.0)  < 15.0;
  bool near270 = headingErrorDeg(heading, 270.0) < 15.0;
  if (!near90 && !near270 && !force) return;

  double targetHeading = near90 ? 90.0 : 270.0;

  Wall wall = chooseWall(leftwall,leftPositive, rightPositive);
  if (wall == Wall::None && !force) return;

  double newY = pose.y;

  // ---- LEFT WALL (same logic as onMatchLoadReset_fixed) ----
  if (wall == Wall::Left || force) {
    double corrected = correctedWallDistanceInches(
        distLeft.get(), heading, targetHeading
    );

    if (leftPositive) {
      newY = 70.5 - OFFSET_LEFT - corrected;
    } else {
      newY = -70.5 + OFFSET_LEFT + corrected;
    }

#ifdef ODOMRESET_DEBUG
    pros::lcd::print(3, "Cosine Left: corr=%.2f offset=%.2f newY=%.2f",
                     corrected, OFFSET_LEFT, newY);
#endif

    chassis.setPose(pose.x, newY, pose.theta);
    pros::delay(50);
    return;
  }

  // ---- RIGHT WALL (same logic as onMatchLoadReset_fixed) ----
  if (wall == Wall::Right) {
    double corrected = correctedWallDistanceInches(
        distRight.get(), heading, targetHeading
    );

    if (rightPositive) {
      newY = 70.5 - OFFSET_RIGHT - corrected;

    } else {
      newY = -70.5 + OFFSET_RIGHT + corrected;
    }

#ifdef ODOMRESET_DEBUG
    pros::lcd::print(3, "Cosine Right: corr=%.2f offset=%.2f newY=%.2f",
                     corrected, OFFSET_RIGHT, newY);
#endif

    chassis.setPose(pose.x, newY, pose.theta);
  }

  pros::delay(50);
}

void resetposition(bool leftwall, bool leftPositive, bool rightPositive){
  resetPoseWithCosineCorrection(leftwall, leftPositive, rightPositive, false);
}

void resetposition(){
  resetPoseWithCosineCorrection(true,true, true, false);
}