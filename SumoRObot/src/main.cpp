#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// define the pins
# define LeftSerrvoPIN 9
# define RightSerrvoPIN 10
# define LMotorA A0
# define LMotorB A1
# define LMotorPWM 5
# define RMotorA A2
# define RMotorB A3
# define RMotorPWM 6

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

const byte address[6] = "01011";

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

void setServoPosition() {
  // Set the servo position based on the joystick position
  LeftServo.write(map(data.j1PotX, 0, 255, 0, 180));
  RightServo.write(map(data.j2PotX, 0, 255, 0, 180));
}

void setMotorSpeed() {
  // Set the motor speed based on the joystick position
  if (data.j1PotY > 127) {
    analogWrite(LMotorPWM, map(data.j1PotY, 127, 255, 0, 255));
    digitalWrite(LMotorA, HIGH);
    digitalWrite(LMotorB, LOW);
  } else {
    analogWrite(LMotorPWM, map(data.j1PotY, 0, 127, 0, 255));
    digitalWrite(LMotorA, LOW);
    digitalWrite(LMotorB, HIGH);
  }

  if (data.j2PotY > 127) {
    analogWrite(RMotorPWM, map(data.j2PotY, 127, 255, 0, 255));
    digitalWrite(RMotorA, HIGH);
    digitalWrite(RMotorB, LOW);
  } else {
    analogWrite(RMotorPWM, map(data.j2PotY, 0, 127, 0, 255));
    digitalWrite(RMotorA, LOW);
    digitalWrite(RMotorB, HIGH);
  }  
  
}

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
  pinMode(LMotorPWM, OUTPUT);
  pinMode(RMotorPWM, OUTPUT);
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
  
   // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }

  setServoPosition();
  setMotorSpeed();
  

}
