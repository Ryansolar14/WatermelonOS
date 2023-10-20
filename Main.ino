#include <IBusBM.h>
#include <Servo.h>

IBusBM IBus;

const int rightMotorPin = 4;
const int leftMotorPin = 5;
const int weaponMotorPin = 6;

Servo rightMotor;
Servo leftMotor;
Servo weaponMotor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting....");
  IBus.begin(Serial1);
  Serial.println("Wait for reciever.");
  while (IBus.cnt_rec==0){
    delay(100);
  }
  Serial.println("Init done");

  rightMotor.attach(rightMotorPin);
  leftMotor.attach(leftMotorPin);
  weaponMotor.attach(weaponMotorPin);

}

int savespd=0, saveturn=0, savefwd=0;

void loop() {
  long spd, turn, fwd, left, right, weapon;
  // speed depends on front switch (channel 5) (forward/backwards) and channel 2 (speed)
  spd = ((int) IBus.readChannel(2)); 
  
  // turn depends on channel 0, scale down to -200, +200
  turn = (((int) IBus.readChannel(0)-1500)); 
  fwd = (((int) IBus.readChannel(1)-1500));

  left = -1*(fwd - turn);
  right = fwd + turn;

  long iMax = max(abs(left), abs(right));

  if(iMax > 500){
    left = (left * 500) / iMax;
    right = (right * 500) / iMax;
  }

  left += 1500;
  right += 1500;
  weapon = spd;

  leftMotor.writeMicroseconds(left);
  rightMotor.writeMicroseconds(right);
  weaponMotor.writeMicroseconds(weapon);

  if (savespd != spd || saveturn != turn || savefwd != fwd) {
    Serial.print("speed="); 
    Serial.println(spd); // display speed
    Serial.print(" turn="); 
    Serial.println(turn); // display turn 
    Serial.print(" fwd="); 
    Serial.println(fwd); // display turn
    Serial.print(" left="); 
    Serial.println(left); 
    Serial.print(" right="); 
    Serial.println(right); 
    Serial.print(" weapon="); 
    Serial.println(weapon); 
    savefwd = fwd;
    savespd = spd;
    saveturn = turn;
  }
  delay(100);

}
