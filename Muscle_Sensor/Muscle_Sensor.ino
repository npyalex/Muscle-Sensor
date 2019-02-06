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

#include "config.h" // change local wifi settings in the config.h tab

int sensorPin = A0;    // select the input pin for the muscle sensor
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int lastRead;         //variable to hold the timer's value
int sampleRate = 10000; //check the sensor every 10 seconds
int threshold = 100; //the threshold for a "tense" reading

bool tense = false;   //is the sensor detecting tension?
bool lastTension = false; //if the last reading matches, activate IFTTT

//send to Adafruit feed called "muscleManager"
AdafruitIO_Feed *muscleManager = io.feed("muscleManager"); 

void setup() {
  // any pin setup happens here
  pinMode(sensorPin, INPUT); //read the muscle sensor
  pinMode(ledPin, OUTPUT); //turn on to indicate tension
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

  // if the value is above the treshold, turn the LED on
  // and the "tense" boolean is "true"
  if(sensorValue>threshold){
    tense = true;
    digitalWrite(ledPin,HIGH);
  } else {
  // if not, LED off and "tense" is "false"
    tense = false;
    digitalWrite(ledPin,LOW);
  }
  // every 10 seconds, check to see if tension is detected
  // if it's detected twice in a row, send a message to Adafruit IO
  if(millis()- lastRead >= sampleRate) {
    //print the sensorValue in the Serial port so it can be monitored
    Serial.print(sensorValue);  
    //if "tense" was true twice in a row, send that information to Adafruit IO
    if(lastTension==true){
      Serial.print("transmitting ");
      Serial.println(lastTension);
      muscleManager->save(lastTension);
    }
    //assign the current reading to "lastTension"
    lastTension = tense;
    //assign the current millisecond count to lastRead for timing purposes
    lastRead = millis();
  }
}
