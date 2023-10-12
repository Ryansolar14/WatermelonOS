#include <IBusBM.h>

IBusBM IBus;

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

}

int savespd=0, saveturn=0;

void loop() {
  int spd, turn;
  // speed depends on front switch (channel 5) (forward/backwards) and channel 2 (speed)
  spd = ((int) IBus.readChannel(2)-1050); 
  // every value below 1050 we interprete as stop 
  if (spd<0) spd=0; else spd = (spd*4)/9; // value could reach (2000-1050)*4/9 = 422, but setspeed() will max at 400
  if (IBus.readChannel(5)>1500) spd=-spd; // backward/forward depends on switch at channel 5
  
  // turn depends on channel 0, scale down to -200, +200
  turn = (((int) IBus.readChannel(0)-1500)*4)/10; 

  // set combined speed and turn (if speed==0, then only turn in place)
  

  if (savespd != spd || saveturn != turn) {
    Serial.print("speed="); Serial.print(spd); // display speed
    Serial.print(" turn="); Serial.println(turn); // display turn 
    savespd = spd;
    saveturn = turn;
  }
  delay(100);
}
