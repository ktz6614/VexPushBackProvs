#include "lemlib/api.hpp" // IWYU pragma: keep
extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;
extern pros::Rotation vertical_encoder;
extern pros::Imu imu;
lemlib::Drivetrain drivetrain(&left_mg,&right_mg,12,lemlib::Omniwheel::NEW_325,360,2);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, 2.72
,0);
lemlib::OdomSensors sensors(&vertical_tracking_wheel,nullptr,nullptr, nullptr,&imu);
lemlib::ControllerSettings lateral_controller(4, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              14, // derivative gain (kD)
                                              0, // anti windup
                                              0.5, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              300, // large error range timeout, in milliseconds
                                              30 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2.6, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              21, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              4, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              30 // maximum acceleration (slew)
);
lemlib::Chassis chassis(drivetrain, // drivetrain settings
    lateral_controller, // lateral PID settings
    angular_controller, // angular PID settings
    sensors // odometry sensors
);