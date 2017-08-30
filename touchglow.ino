/**
    Make the white LEDs on a GRBW NeoPixel strip connected to a FLORA
    grow brighter over time.

    And yeah, let's do that to the on-board RGB pixel, too!
*/
#include <TweenDuino.h>
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

// Name some pins.
const int redLED = 7; // On-board red LED.
const int onBoardLED = 8;  // Flora's NeoPixel is on Pin 8.
const int LEDStrip = 9; // My pixel strip's data line is on pin 9.


CapacitiveSensor   cs_2_3 = CapacitiveSensor(3,2);        // 10M resistor between pins 2 & 3, pin 2 is sensor pin, add a wire and or foil if desired
Adafruit_NeoPixel boardLED = Adafruit_NeoPixel(1, onBoardLED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripLEDs = Adafruit_NeoPixel(7, LEDStrip, NEO_GRBW);

// Start dim (0).
float brightness = 0.0;

TweenDuino::Timeline timeline;

void setup() {
  
    Serial.begin(9600);

    boardLED.begin();
    stripLEDs.begin();

    cs_2_3.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example

    makeSOSTimeline(timeline);

    // These might help keep us from going blind during dev by limiting the max brightness.
    // Note: You shouldn't be using this in NeoPixel animations.  Call it once.
    //boardLED.setBrightness(100);
    //stripLEDs.setBrightness(100);

    // Initialize the neoPixels to be "black", clearing out any color info
    // stored from before a reset or a crash.
    boardLED.show();
    stripLEDs.show();

    delay(4000);
}
void loop() {
    long loopStart = millis();
    long total1 = 0;
    //long total1 =  cs_2_3.capacitiveSensor(30);
    //Serial.println(total1);  //Serial.println(brightness);
    //Serial.println(loopStart);
    timeline.update(loopStart);

    if (timeline.isComplete()) {
      brightness = 0.0;
    }
    
    // Override brightness if touching
    if (total1 > 700) {
      brightness = 255.0;
    }

    // 3 colors: Red, Green, Blue whose values range from 0-255.
    const uint32_t boardColor = boardLED.Color(0, brightness, 0); // Somewhat bright green.
    const uint32_t stripColor = stripLEDs.Color(0, 0, 0, brightness); // Colors are off, white LED is pure white.
    
    setStripColors(boardLED, boardColor);
    setStripColors(stripLEDs, stripColor);

    // This sends the updated pixel color to the hardware.
    stripLEDs.show();
    boardLED.show();
}

void setStripColors(Adafruit_NeoPixel &strip, uint32_t color) {
    const int numPixels = strip.numPixels();

    for (int i=0;i<numPixels;i++) {
        strip.setPixelColor(i, color);
    }
}


void makeSOSTimeline(TweenDuino::Timeline &timeline) {
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,500UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,500UL, 0.0));
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,500UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,500UL, 0.0));
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,500UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,500UL, 0.0));
}
