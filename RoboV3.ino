#include <Ultrasonic.h>
#include <Servo.h>

const int trigPin = 5;
const int echoPin = 4;
const int enA = 11;
const int enB = 6;
const int motor1PinA = 13;
const int motor1PinB = 12;
const int motor2PinA = 8;
const int motor2PinB = 7;
const int servoPin = 3;
const int obstacleDistance = 15;

Ultrasonic ultrasonic(trigPin, echoPin);
Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(motor1PinA, OUTPUT);
  pinMode(motor1PinB, OUTPUT);
  pinMode(motor2PinA, OUTPUT);
  pinMode(motor2PinB, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  analogWrite(enA, 60);
  analogWrite(enB, 60);
  servo.attach(servoPin);
  servo.write(80);
}

void loop() {
  int distance = ultrasonic.read();
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= obstacleDistance) {
    stopAndCheckObstacle();
  } else {
    moveForward();
  }
}

void moveForward() {
  digitalWrite(motor1PinA, HIGH);
  digitalWrite(motor1PinB, LOW);
  digitalWrite(motor2PinA, HIGH);
  digitalWrite(motor2PinB, LOW);
}

void stopMotors() {
  digitalWrite(motor1PinA, LOW);
  digitalWrite(motor1PinB, LOW);
  digitalWrite(motor2PinA, LOW);
  digitalWrite(motor2PinB, LOW);
}

void stopAndCheckObstacle() {
  // Para completamente os motores
  stopMotors();

  // Gira o sensor para a direita
  servo.write(0);
  delay(500);

  // Verifica a distância à direita
  int rightDistance = ultrasonic.read();
  Serial.print("Distância à direita: ");
  Serial.print(rightDistance);
  Serial.println(" cm");

  // Gira o sensor para a esquerda
  servo.write(180);
  delay(500);

  // Verifica a distância à esquerda
  int leftDistance = ultrasonic.read();
  Serial.print("Distância à esquerda: ");
  Serial.print(leftDistance);
  Serial.println(" cm");

  // Volta o sensor para a frente
  servo.write(80);

  // Tomada de decisão com base nas leituras à direita e à esquerda
  if (rightDistance > leftDistance) {
    // Se o espaço à direita estiver mais livre, vira para a direita
    turnRight();
  } else {
    // Caso contrário, vira para a esquerda
    turnLeft();
  }
}

void turnRight() {
  // Gira para a direita
  digitalWrite(motor1PinA, LOW);
  digitalWrite(motor1PinB, HIGH);
  digitalWrite(motor2PinA, HIGH);
  digitalWrite(motor2PinB, LOW);
  delay(1000);

  // Volta a seguir em frente
  moveForward();
}

void turnLeft() {
  // Gira para a esquerda
  digitalWrite(motor1PinA, HIGH);
  digitalWrite(motor1PinB, LOW);
  digitalWrite(motor2PinA, LOW);
  digitalWrite(motor2PinB, HIGH);
  delay(1000);

  // Volta a seguir em frente
  moveForward();
}
