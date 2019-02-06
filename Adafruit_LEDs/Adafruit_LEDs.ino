//Import the needed FastLED library
#include <FastLED.h>
 
//These variables allow you to specify how the strip will behave
#define NUM_LEDS 50  //This number decides how many of the strip's LEDs the code will use
const int ledLength = 8; //This number decides how many LEDs will be turned on at once
 
//Define and initiate global variables
CRGB leds[NUM_LEDS];
#define DATA_PIN 9
int sensorPin = A0;
int ledValue; 
int sensorValue;
 
void setup() {
  // initiate the LED strip
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}
 
void loop() {
  //Read value from any sensor
  sensorValue = analogRead(sensorPin);  
 
  //Using the map function, we can take any sensor input and align it to a location on the LED strip
  ledValue = map(sensorValue, 0, 1023, 0, NUM_LEDS);
  
  //Clear past LED configuration and reset fade
  FastLED.clear();
 
  //This code makes the light appear one at a time in the beginning of the strip. 
  if(ledValue <= ledLength){
    for(int led = 0; led <= ledValue; led++) { 
            leds[led] = CRGB::White;
            }
   FastLED.show();
  }
  
  //This code turns on the lights which fall in the values specified by the user. 
  else {
    for(int led = ledValue-ledLength; led < ledValue; led++) { 
      leds[led] = CRGB::White; 
    }
  FastLED.show();
 }
}
