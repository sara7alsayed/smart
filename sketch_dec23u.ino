#include <Servo.h>

Servo Myservo;

#define trigPin 9           // Trig Pin Of HC-SR04
#define echoPin 8           // Echo Pin Of HC-SR04
#define MLa 4               // left motor 1st pin
#define MLb 5               // left motor 2nd pin
#define MRa 6               // right motor 1st pin
#define MRb 7               // right motor 2nd pin
#define LED 13
int Speed = 204;            // 0 - 255.
int Speedsec;
int Turnradius = 0;          // Set the radius of a turn, 0 - 255
int brakeTime = 45;
int brkonoff = 1;           // 1 for the electronic braking system, 0 for normal.

#define in1 7                // L298n Motor Driver pins.
#define in2 6
#define in3 5
#define in4 4

int command;                // Int to store app command state.
int buttonState = 0;
int lastButtonState = 0;

void setup() {
  pinMode(MLa, OUTPUT);       // Set Motor Pins As O/P
  pinMode(MLb, OUTPUT);
  pinMode(MRa, OUTPUT);
  pinMode(MRb, OUTPUT);

  pinMode(trigPin, OUTPUT);   // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin, INPUT);    // Set Echo Pin As I/P To Receive Reflected Waves
  Myservo.attach(10);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(LED, OUTPUT);        // Set the LED pin.
  Serial.begin(9600);         // Set the baud rate to your Bluetooth module.
}

void loop() {
  // Check for Bluetooth command
  if (Serial.available() > 0) {
    command = Serial.read();
    Stop(); // Initialize with motors stopped.

    switch (command) {
      case 'F': {
        forward();
        break;
      }
      case 'B': {
        back();
        break;
      }
      case 'L': {
        left();
        break;
      }
      case 'R': {
        right();
        break;
      }
      case 'G': {
        forwardleft();
        break;
      }
      case 'I': {
        forwardright();
        break;
      }
      case 'H': {
        backleft();
        break;
      }
      case 'J': {
        backright();
        break;
      }
      case '0': {
        Speed = 100;
        break;
      }
      case '1': {
        Speed = 140;
        break;
      }
      case '2': {
        Speed = 153;
        break;
      }
      case '3': {
        Speed = 165;
        break;
      }
      case '4': {
        Speed = 178;
        break;
      }
      case '5': {
        Speed = 191;
        break;
      }
      case '6': {
        Speed = 204;
        break;
      }
      case '7': {
        Speed = 216;
        break;
      }
      case '8': {
        Speed = 229;
        break;
      }
      case '9': {
        Speed = 242;
        break;
      }
      case 'q': {
        Speed = 255;
        break;
      }
    }

    Speedsec = Turnradius;
    if (brkonoff == 1) {
      brakeOn();
    } else {
      brakeOff();
    }
  } else {
    // If no Bluetooth command, perform obstacle avoidance
    obstacleAvoidance();
  }
}

void obstacleAvoidance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);    // Transmit Waves For 10us
  delayMicroseconds(10);
  long duration = pulseIn(echoPin, HIGH); // Receive Reflected Waves
  long distance = duration / 58.2;       // Get Distance

  Serial.println(distance);
  delay(10);

  if (distance > 40) {
    // No obstacle, move forward
    Myservo.write(90);
    digitalWrite(MRb, HIGH);       // Move Forward
    digitalWrite(MRa, LOW);
    digitalWrite(MLb, HIGH);
    digitalWrite(MLa, LOW);
  } else {
    // Obstacle detected, perform avoidance behavior
    digitalWrite(MRb, LOW);        // Stop
    digitalWrite(MRa, LOW);
    digitalWrite(MLb, LOW);
    digitalWrite(MLa, LOW);
    delay(100);

    Myservo.write(0);
    delay(500);
    Myservo.write(180);
    delay(500);
    Myservo.write(90);
    delay(500);

    digitalWrite(MRb, LOW);        // Move Backward
    digitalWrite(MRa, HIGH);
    digitalWrite(MLb, LOW);
    digitalWrite(MLa, HIGH);
    delay(500);

    digitalWrite(MRb, LOW);        // Stop
    digitalWrite(MRa, LOW);
    digitalWrite(MLb, LOW);
    digitalWrite(MLa, LOW);
    delay(100);

    digitalWrite(MRb, HIGH);       // Move Left
    digitalWrite(MRa, LOW);
    digitalWrite(MLa, LOW);
    digitalWrite(MLb, LOW);
    delay(500);
  }
}

void forward() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speed);
}

void back() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speed);
}

void left() {
  analogWrite(in3, Speed);
  analogWrite(in2, Speed);
}

void right() {
  analogWrite(in4, Speed);
  analogWrite(in1, Speed);
}

void forwardleft() {
  analogWrite(in1, Speedsec);
  analogWrite(in3, Speed);
}

void forwardright() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speedsec);
}

void backright() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speedsec);
}

void backleft() {
  analogWrite(in2, Speedsec);
  analogWrite(in4, Speed);
}

void Stop() {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void brakeOn() {
  // Here's the future use: an electronic braking system!
  // read the pushbutton input pin:
  buttonState = command;
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == 'S') {
      if (lastButtonState != buttonState) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, HIGH);
        delay(brakeTime);
        Stop();
      }
    }
    // save the current state as the last state,
    // for next time through the loop
    lastButtonState = buttonState;
  }
}
void brakeOff() {
  // Release the electronic brake
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
