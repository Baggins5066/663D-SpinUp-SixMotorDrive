#include "main.h"
using namespace okapi;

//Controller Setup:
Controller controller;

//Slew Setup
	void Slew_left(void* motor_ptr) {
		MotorGroup* motor = (MotorGroup*) motor_ptr;
		while(1) {
			int target = 12000 * ((controller.getAnalog(ControllerAnalog::leftY)) + (controller.getAnalog(ControllerAnalog::rightX)));
			// std::cout << target << "\n";
			if (std::abs(target - motor->getVoltage()) < 3900) {
				motor->moveVoltage(target);
			}
			else {
				if (motor->getVoltage() < target) {
					motor->moveVoltage(motor->getVoltage() + 3900);
				}
				else {
					motor->moveVoltage(motor->getVoltage() - 3900);
				}
			}
			pros::delay(50);
			}
	}

	void Slew_right(void* motor_ptr) {
		MotorGroup* motor = (MotorGroup*) motor_ptr;
		while(1) {
			int target = 12000 * ((controller.getAnalog(ControllerAnalog::leftY)) - (controller.getAnalog(ControllerAnalog::rightX)));
			if (std::abs(target - motor->getVoltage()) < 3900) {
				motor->moveVoltage(target);
			}
			else {
				if (motor->getVoltage() < target) {
					motor->moveVoltage(motor->getVoltage() + 3900);
				}
				else {
					motor->moveVoltage(motor->getVoltage() - 3900);
				}
			}
			pros::delay(50);
			}
	}

//Motor Setup:
	bool FlywheelSpeed = 0;
	okapi::Motor Intake(-14);
	//okapi::Motor Flywheel(-10, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	okapi::Motor Flywheel(-10);
	pros::Imu Inertial (0);

//Pneumatics Setup:
	pros::ADIDigitalOut Endgame ('G');
	pros::ADIDigitalOut Plate ('A');

bool PlateVar = true;

void TogglePlate(){
	Plate.set_value(true);
	pros::delay(1000);
	Plate.set_value(false);
}
void ShootEndgame(){
	Endgame.set_value(true);
	}

//Chassis Setup:
	std::shared_ptr<ChassisController> chassis =
	ChassisControllerBuilder()
	.withMotors({20, 19, -18}, {-11,-12, 13}) //Defines drive train motors
	.withDimensions({AbstractMotor::gearset::blue}, {{3_in, 15_in}, imev5GreenTPR}) //Defines robot dimensions and gear set
	.build();

//Maximun Velocity Setup
int maxVel = chassis->getMaxVelocity();

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "That tickles!");
	} else {
		pros::lcd::clear_line(2);
	}}

void initialize() {
	pros::lcd::initialize(); //Runs initialization code. This occurs as soon as the program is started. All other competition modes are blocked by initialize; it is recommendedto keep execution time for this mode under a few seconds.
	pros::lcd::register_btn1_cb(on_center_button);
	pros::ADIDigitalOut piston1 ('D');
	pros::ADIDigitalOut piston2 ('B');
}

void disabled() {}

void competition_initialize();

void autonomous() {
	//Rename programs in "project.pros
	//Skills
	chassis->setMaxVelocity(100);
	chassis->moveDistance(-3_in);
	Intake.moveVoltage(12000);
	pros::delay(500);
	Intake.moveVoltage(0);
}

void opcontrol() {
	MotorGroup left{20, -19, -18};
	MotorGroup right{-11, 12, 13};
	pros::Task Left_task (Slew_left, &left);
	pros::Task Right_task (Slew_right, &right);

	chassis->getModel()->setBrakeMode(AbstractMotor::brakeMode::coast); //Sets starting brake mode to Coast
	int FlywheelSpeed = 12000;
	pros::delay(500);
	pros::lcd::set_text(2, "FlywheelSpeed");
	while(true){
		Flywheel.setBrakeMode(AbstractMotor::brakeMode::coast);
		Intake.setBrakeMode(AbstractMotor::brakeMode::coast);
		//Button Declaration
			ControllerButton R1(ControllerDigital::R1);
			ControllerButton R2(ControllerDigital::R2);
			ControllerButton L2(ControllerDigital::L2);
			ControllerButton L1(ControllerDigital::L1);
			ControllerButton X(ControllerDigital::X);
			ControllerButton B(ControllerDigital::B);
			ControllerButton A(ControllerDigital::A);
			ControllerButton Y(ControllerDigital::Y);
			ControllerButton down(ControllerDigital::down);
			ControllerButton right(ControllerDigital::right);
		Flywheel.moveVoltage(FlywheelSpeed);
		
		/*Intake Controls*/
		if(R1.isPressed()) {
			Intake.moveVoltage(-12000);
		}
		else {
			Intake.moveVoltage(12000);
		}

		/*Flywheel Toggle*/
		if(L1.changedToPressed()) {
			FlywheelSpeed = 12000;
		}
		if(L2.changedToPressed()) {
			FlywheelSpeed = 0;
		}

		/*Plate controls*/
		if(right.isPressed()) {
			Plate.set_value(false);
			}
		else {
			Plate.set_value(true);
		}

		/*Endgame controls*/
		if(Y.isPressed()) {
			ShootEndgame();
			}
		
		/*Flywheel throttle*/
		// if(L1.isPressed()) {
		// 	FlywheelSpeed = (FlywheelSpeed + 1);
		// }
		// else if(L2.isPressed()) {
		// 	FlywheelSpeed = (FlywheelSpeed - 1);
		// }

		/*Flywheel limit*/
		// if(FlywheelSpeed < 0) {
		// 	FlywheelSpeed = 0;
		// }
		// if(FlywheelSpeed > 12000) {
		// 	FlywheelSpeed = 12000;
		// }

		/*Velocity controls*/
		// if (A.isPressed()) {
		// 	chassis->setMaxVelocity((maxVel * 1)); //Sets maximum velocity
		// }
		// if (Y.isPressed()) {
		// 	chassis->setMaxVelocity((maxVel * .25)); //Sets maximum velocity
		// }
	}

pros::delay(20); //Adds a delay to make sure the computer can keep up with the program
}