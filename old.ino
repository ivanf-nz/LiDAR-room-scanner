#include <AccelStepper.h>
#include <Wire.h>
#include <TFLI2C.h>

// Stepper motor setup
const int stepMode = 8; // 1/8 step mode
const int stepsPerRev = 200; // Full step mode steps per revolution
const float stepAngle = 360.0 / (stepsPerRev * stepMode);

// NEMA 17 motor 1
const int dirPin1 = 2;
const int stepPin1 = 3;
AccelStepper stepper1(AccelStepper::DRIVER, stepPin1, dirPin1);

// NEMA 17 motor 2
const int dirPin2 = 4;
const int stepPin2 = 5;
AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);

// KY-003 Hall effect sensors
const int hallPin1 = 6; // Sensor for stepper1
const int hallPin2 = 7; // Sensor for stepper2

// TF Luna LIDAR
TFLI2C tflI2C;
int16_t tfDist; // Distance in centimeters
const int16_t tfAddr = TFL_DEF_ADR; // Default I2C address for LIDAR

// Motion parameters
const int maxSteps2 = (int)((180.0 / 360.0) * stepsPerRev * stepMode); // Maximum steps for stepper2 to cover 180 degrees
float speed2 = 50.0; // Speed of stepper2 in steps per second

// Function prototypes
void homeMotors();
float readLidar();
void rotateMotor(AccelStepper &motor, int steps);

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Initialize Wire library

  // Initialize LIDAR
  if (!tflI2C.getData(tfDist, tfAddr)) {
    Serial.println("Failed to initialize LIDAR sensor.");
  }

  // Initialize steppers
  stepper1.setMaxSpeed(1000);
  stepper2.setMaxSpeed(speed2);

  // Set initial speeds
  stepper1.setAcceleration(500);
  stepper2.setAcceleration(500);

  // Initialize Hall effect sensors
  pinMode(hallPin1, INPUT);
  pinMode(hallPin2, INPUT);

  // Home the motors
  homeMotors();
}

void loop() {
  // Main scanning loop
  for (int step2 = 0; step2 <= maxSteps2; step2++) {
    // Take LIDAR measurement
    float distance = readLidar();
    Serial.print("Angle1: ");
    Serial.print(stepper1.currentPosition() * stepAngle);
    Serial.print("\tAngle2: ");
    Serial.print(step2 * stepAngle);
    Serial.print("\tDistance: ");
    Serial.println(distance);

    // Rotate stepper2 by one full step
    rotateMotor(stepper2, 1);
  }

  // Return stepper2 to home position
  rotateMotor(stepper2, -maxSteps2);

  // Rotate stepper1 by one full step
  rotateMotor(stepper1, 1);
}

void homeMotors() {
  // Home stepper1
  stepper1.setSpeed(-100);
  while (digitalRead(hallPin1) == LOW) {
    stepper1.runSpeed();
  }
  stepper1.setCurrentPosition(0);

  // Home stepper2
  stepper2.setSpeed(-100);
  while (digitalRead(hallPin2) == LOW) {
    stepper2.runSpeed();
  }
  stepper2.setCurrentPosition(0);
}

float readLidar() {
  // Read LIDAR distance
  if (tflI2C.getData(tfDist, tfAddr)) {
    return tfDist; // Return distance in cm
  } else {
    Serial.println("Failed to read LIDAR data.");
    return -1; // Indicate an error
  }
}

void rotateMotor(AccelStepper &motor, int steps) {
  motor.move(steps * stepMode);  // Multiply steps by stepMode to account for microstepping
  while (motor.distanceToGo() != 0) {
    motor.run();
  }
}
