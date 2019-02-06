/*
  Muscle Tension Sensor
``Measure your tension and send a notification to your phone
  over AdafruitIO & IFTTT
  
  for Body-centric Technologies
  profs Lee Jones & Kate Hartman
  by Nick Alexander
  OCADU Digital Futures MDes 2019

  Based on DIY Analog Input
  http://www.arduino.cc/en/Tutorial/AnalogInput
  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe
  and
  Adafruit IO Digital Input Example
  https://learn.adafruit.com/adafruit-io-basics-digital-input
*/

#include "config.h"

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int lastRead;
int sampleRate = 10000; //check the sensor every 10 seconds

bool tense = false;
bool last = false;
bool lastTension = false;

AdafruitIO_Feed *digital = io.feed("digital");

void setup() {
  // any pin setup happens here
  pinMode(sensorPin, INPUT);
  // start the serial connection
  // note that it's 115200
  Serial.begin(115200);
  // wait for the connection to Adafruit IO  
  while(!Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // confirm connection
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  //io.run() is required at the top of the loop
  io.run();
  
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);

  if(sensorValue>100){
    tense = true;
  } else {
    tense = false;
  }
  // every 10 seconds, check to see if tension is detected
  // if it's detected twice in a row, send a message to Adafruit IO
  if(millis()- lastRead >= sampleRate) {
    if(lastTension==true){
      Serial.print("transmitting ");
      Serial.println(lastTension);
      digital->save(lastTension);
    }
    lastTension = tense;
    lastRead = millis();
  }
}
