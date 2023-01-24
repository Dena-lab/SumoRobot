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

struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};

Data_Package data;

const byte address[6] = "01011";      // 11

void setup() {

  // intialize the NRF24L01 module as reciever
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();

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


void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}