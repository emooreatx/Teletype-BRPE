//#include <SoftwareSerial.h>
#include "Tlc5940.h"

//SoftwareSerial mySerial(13, 12); // RX, TX

unsigned long interval=8; // the time between sprockets
unsigned long previousMillis=0; // millis() returns an unsigned long.

byte mybyte;
bool dataready;
int ledcount;
int sizeofprog = 0;
bool atspeed = 0;

void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
  Tlc.init();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  
  loop();

}




/*
  for(int x = 2; x < 6; x++)
  {
    digitalWrite(x, LOW);

  }
  delay(100);
  for(int x = 2; x < 6; x++)
  {
    digitalWrite(x, HIGH);
  }
  digitalWrite(10, LOW);
  delay(100);
  digitalWrite(10, HIGH);
  delay(1000);
  for(int x = 6; x < 10; x++)
  {
    digitalWrite(x, LOW);

  }
  delay(100);
  for(int x = 6; x < 10; x++)
  {
    digitalWrite(x, HIGH);
  }
  delay(100);
  digitalWrite(10, LOW);
  delay(100);
  digitalWrite(10, HIGH);
  delay(1000);
*/
/*  if (mySerial.available()) {
    delay(3000);
    sizeofprog = mySerial.read() + 5;
    readprogram();
  }
*/


int incomingByte = 0; // for incoming serial data



void loop() {
  digitalWrite(10, HIGH);
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("MAIN LOOP I received: ");
    Serial.println(incomingByte, DEC);
    if (incomingByte == 65) {
      Serial.print("MAIN LOOP Starting Readprogram\n");
      readprogram();
    }
  }
}

void readprogram() {
  bool reading = 0;
  int incomingbyte = 0;
  while(!reading) {
    if (Serial.available() > 0) {
      Serial.print("WAITING FOR A Data Received");
      incomingbyte = Serial.read();
      if (incomingbyte == 65) {
        Serial.print("That was a second A! Starting read");
        reading = 1;
      }
    }
  }
  int arraycount = 0;
  int sizeofprog = 0;
  bool nosize = 1;

  while(reading && nosize) {
    if (sizeofprog == 0 & Serial.available() > 0) {
      sizeofprog = Serial.read();
      Serial.print("Size of Program: ");
      Serial.println(sizeofprog, DEC);
      nosize = 0;
    }
  }
  byte myprog[sizeofprog];
  while(reading && !nosize){
    if (arraycount < sizeofprog && Serial.available() > 0) {
      myprog[arraycount++] = Serial.read();
      Serial.println(myprog[arraycount-1], DEC);
      if (arraycount == sizeofprog) {
        reading = 0;
      }
    }
  }
  Serial.print("Done reading");
  Serial.println(sizeof(myprog));
  Serial.print("Testing for speed");
  while(atspeed == 0){
    if(digitalRead(11)){
     unsigned long currentMillis = millis();
      if((currentMillis-previousMillis) == 9); {
        //We are at speed
        atspeed = 1;
      }
      previousMillis = currentMillis;
      delay(2);
    }
  }
  Serial.print("At Speed, starting punch");
  paperout(myprog, sizeof(myprog));
}


void paperout2(byte myprog[], int sizeofprog) {
  int arraycount = 0;
  while(arraycount < sizeofprog){ // still more to write
    unsigned long currentMillis = millis(); 
    if ((unsigned long)(currentMillis - previousMillis) >= interval && digitalRead(11)) { //time to write a byte!
      previousMillis = millis();
      for (int x = 9; x >1; x--) {
        if(myprog[arraycount] & 0x01) {
          digitalWrite(x, LOW);
        }
        myprog[arraycount] = myprog[arraycount] >> 1;
      }
      digitalWrite(10, LOW);
      delay(6);
      for (int x = 2; x < 10; x++) { // first 4 off
        digitalWrite(x, HIGH);
      }
      digitalWrite(10, HIGH);
      arraycount++;
    }
  }
}

void paperout(byte myprog[], int sizeofprog) {
  int arraycount = 0;
  while(arraycount < sizeofprog){ // still more to write
    unsigned long currentMillis = millis(); 
    if ((unsigned long)(currentMillis - previousMillis) >= interval && digitalRead(11)) { //time to write a byte!
      previousMillis = millis();
      
      for (int x = 2; x < 6; x++) { // first 4 on
        if(myprog[arraycount] & 0x01) {
          digitalWrite(x, LOW);
        }
        myprog[arraycount] = myprog[arraycount] >> 1;
      }
      delay(2);

      while(!digitalRead(11)){
      }
      
      for (int x = 2; x < 6; x++) { // first 4 off
        digitalWrite(x, HIGH);
      }
      delay(2);
      
      while(!digitalRead(11)){
      }

      for (int x = 6; x < 10; x++) { // second 4 on
        if(myprog[arraycount] & 0x01) {
          digitalWrite(x, LOW);
        }
        myprog[arraycount] = myprog[arraycount] >> 1;
      }
      delay(2);
      
      while(!digitalRead(11)){
      }
      
      for (int x = 6; x < 10; x++) { // second 4 off
        digitalWrite(x, HIGH);
      }
      delay(2);

      while(!digitalRead(11)){
      }
    
      digitalWrite(10, LOW); // sprocket
      delay(2);
      while(!digitalRead(11)){
      }
      digitalWrite(10, HIGH);
      delay(2);
      arraycount++;
    }
  }
}
