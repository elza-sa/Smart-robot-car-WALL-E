#include <LiquidCrystal.h>
#include <Servo.h>

// LCD RS = 12, E = 11, D4 = A1, D5 = A2, D6 = A3, D7 = A4
LiquidCrystal lcd(12, 11, A1, A2, A3, A4);

// Motor/Sensor Pins
#define motor1pin1 8
#define motor1pin2 7
#define motor2pin1 10
#define motor2pin2 9
#define enable1 5
#define enable2 6
#define trigPin 3
#define echoPin 4
#define servopin 2

Servo myservo;
float duration, distance;

void setup() {
  // Setup LCD
  lcd.begin(16, 2);
  lcd.print("WALL-E");

  // Setup motors
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);

  // Setup ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Setup servo
  myservo.attach(servopin);
  myservo.write(90);  // center

  Serial.begin(9600);
  delay(2000);  // Let LCD display "WALL-E" for a moment
  lcd.clear();
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return (duration * 0.0343) / 2;
}

void moveForward() {
  analogWrite(enable1, 100);
  analogWrite(enable2, 100);
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
}

void stopMotors() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
}

void turnLeft() {
  analogWrite(enable1, 200);
  analogWrite(enable2, 200);
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  delay(300);
  stopMotors();
}

void turnRight() {
  analogWrite(enable1, 200);
  analogWrite(enable2, 200);
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  delay(500);
  stopMotors();
}

void loop() {
  distance = getDistance();

  // Display distance on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  Serial.print("Distance ahead: ");
  Serial.println(distance);

  if (distance > 30) {
    moveForward();
  } else {
    stopMotors();

    int bestAngle = 90;
    float maxDistance = 0;

    for (int angle = 0; angle <= 180; angle += 15) {
      myservo.write(angle);
      delay(300);
      float d = getDistance();
      if (d > maxDistance) {
        maxDistance = d;
        bestAngle = angle;
      }
    }

    myservo.write(90);
    delay(300);

    if (bestAngle >= 75 && bestAngle <= 105) {
      delay(300);
    } else if (bestAngle < 75) {
      turnLeft();
    } else {
      turnRight();
    }
  }

  delay(100);
}
