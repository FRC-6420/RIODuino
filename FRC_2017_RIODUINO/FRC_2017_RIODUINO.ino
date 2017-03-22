#define FASTLED_ALLOW_INTERRUPTS 0 //maybe fix the flickering
#include "FastLED.h"
#include <avr/pgmspace.h>
#define COLOR_ORDER GRB
#define MAX_BRIGHTNESS 255
//Tell it how many leds are in the strip. AndyMark's 2.5 meter strip has 150 leds
#define NUM_LEDS 150
// This is an array of leds. One item for each led in your strip
CRGB leds[NUM_LEDS];
//CSK 3/17/2014 I moved this to a pin that doesn't conflict with Ethernet functions in case you want to control LEDs via Ethernet
#define DATA_PIN 6 //White wire from the http://www.andymark.com/product-p/am-2917.htm power connector

unsigned int mode = 0;

//This function is used to setup things like pins, Serial ports etc.
//Here we specify which chipset our LEDs run off of by our choice of config function
void setup()
{
 // Uncomment one of the following lines for your leds arrangement.
 // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
 // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
 // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
 //FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
 // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
 //CSK 2/12/2016 This is the correct chipset for the am-2916 LED strip
 FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
 // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
 //FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
 // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
 // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
 //***This is the chipset in the AM-2640 LED strip***
 //CSK 3/17/2013 Changed to this function to allow direct data and clock pin specification
 //FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
 // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
 // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
 FastLED.clear();
 FastLED.show();
 delay(250);
 //clear() turns all LEDs off
 FastLED.clear();
 FastLED.setBrightness(MAX_BRIGHTNESS);
 fill_solid( leds, NUM_LEDS /*number of leds*/, CRGB( 125, 125, 125) );
 FastLED.show();
 // start serial port at 9600 bps:
 Serial.begin(9600);
 //makelookuptable();
}

void loop()
{
 FastLED.clear();
 delay(50);
 unsigned int curTime = millis();
 for( unsigned int led_number = 0; led_number < NUM_LEDS; led_number++ ){
  switch( mode ){
    case 0: //PULSE_RED
      leds[led_number] = CRGB( fastSin( curTime * 0.005 ), 0, 0 );
      break;
    case 1: //PULSE_GREEN
      leds[led_number] = CRGB( 0, fastSin( curTime * 0.005  ), 0 );
      break;
    case 2: //PULSE_BLUE
      leds[led_number] = CRGB( 0, 0, fastSin( curTime * 0.005 ) );
      break;
    case 3: // SOLID_RED
      leds[led_number] = CRGB( 255, 0, 0 );
      break;
    case 4: // SOLID_GREEN
      leds[led_number] = CRGB( 0, 255, 0 );
      break;
    case 5: // SOLID_BLUE
      leds[led_number] = CRGB( 0, 0, 255 );
      break;
    case 6: // MOVING_RED
      leds[led_number] = CRGB( fastSin(curTime * 0.005 + led_number * 0.5), 0, 0 );
      break;
    case 7: // MOVING_GREEN
      leds[led_number] = CRGB( 0, fastSin(curTime * 0.005 + led_number * 0.5), 0 );
      break;
    case 8: //MOVING_BLUE
      leds[led_number] = CRGB( 0, 0, fastSin(curTime * 0.005 + led_number * 0.5) );
      break;
  }
 }
 FastLED.show();
}

void serialEvent() {
  while (Serial.available()) {
     char c = Serial.read();
     mode = (unsigned int)c;
  }
}

const int max_fastSin_input = 63;
//unsigned int sin_lookup[max_fastSin_input];

const unsigned char sin_lookup[63] = {137, 150, 162, 174, 185, 196, 206, 215, 223, 230, 236, 242, 245, 248, 250, 250, 249, 247, 243, 239, 233, 226, 218, 209, 200, 189, 178, 167, 155, 143, 130, 118, 105, 93, 81, 70, 59, 49, 39, 30, 23, 16, 10, 6, 3, 1, 0, 0, 2, 5, 9, 15, 21, 28, 37, 46, 56, 67, 78, 90, 102, 115, 127 };

inline unsigned int fastSin( float input ){
  unsigned int approximation = ((unsigned int)(input * 10) % max_fastSin_input);
  return sin_lookup[ approximation ];
}


/*void makelookuptable(){
  for(unsigned int x = 0; x < max_fastSin_input; x++ ){
    sin_lookup[ x ] = (sin( x * 0.1 ) + 1) * 125;
  }
}
*/

