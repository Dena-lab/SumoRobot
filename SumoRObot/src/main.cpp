#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// define the pins
# define LeftSerrvoPIN 9
# define RightSerrvoPIN 10
# define LMotorA 3
# define LMotorB 4
# define RMotorA 5
# define RMotorB 6

// define the servos
Servo LeftServo;
Servo RightServo;

RF24 radio(7, 8);   // CE, CSN

const byte address[6] = "01011";      // 11

void setup() {

  // intialize the NRF24L01 module as reciever
  Serial.begin(19200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // intialize the motors
  pinMode(LMotorA, OUTPUT);
  pinMode(LMotorB, OUTPUT);
  pinMode(RMotorA, OUTPUT);
  pinMode(RMotorB, OUTPUT);
  
  // intialize the servos
  LeftServo.attach(LeftSerrvoPIN);
  RightServo.attach(RightSerrvoPIN);

  //set the servos to the default position
  LeftServo.write(90);
  RightServo.write(90);

}

void loop() {
  
  
  
}