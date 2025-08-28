#include <Servo.h>

Servo myServo;       // create a servo object
int servoPin = 9;    // digital pin connected to servo signal wire
int angle = 0;       // starting angle

void setup() {
  myServo.attach(servoPin);  // attach servo to pin 9
}

void loop() {
  // Sweep from 0 to 180 degrees
  for (angle = 0; angle <= 180; angle += 1) {
    myServo.write(angle);
    delay(15); // wait for servo to reach position
  }

  // Sweep back from 180 to 0 degrees
  for (angle = 180; angle >= 0; angle -= 1) {
    myServo.write(angle);
    delay(15);
  }
}
