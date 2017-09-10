/**
    Make the white LEDs on a GRBW NeoPixel strip connected to a FLORA
    grow brighter over time.

    And yeah, let's do that to the on-board RGB pixel, too!
*/

#include <TweenDuino.h>
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>
#include <MemoryFree.h>

#define TOUCH_THRESHOLD 300

// Name some pins.
const int redLED = 7; // On-board red LED.
const int onBoardLEDPin = 8;  // Flora's NeoPixel is on Pin 8.
const int LEDStripPin = 9; // My pixel strip's data line is on pin 9.
const int capSensorPin = 2;
const int capReadPin = 3;

bool shouldReadTouch;

CapacitiveSensor   cs_2_3 = CapacitiveSensor(capReadPin,capSensorPin);        // 10M resistor between pins 2 & 3, pin 2 is sensor pin, add a wire and or foil if desired
Adafruit_NeoPixel boardLED = Adafruit_NeoPixel(1, onBoardLEDPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripLEDs = Adafruit_NeoPixel(7, LEDStripPin, NEO_GRBW);

// Start dim (0).
float brightness = 0.0;

TweenDuino::Timeline timeline;

void setup() {
 
    Serial.begin(9600);
    boardLED.begin();
    stripLEDs.begin();

    cs_2_3.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example

    Serial.print("startmem=");
    Serial.println(freeMemory());
    adil(timeline);
    Serial.print("aftertl=");
    Serial.println(freeMemory());

    // These might help keep us from going blind during dev by limiting the max brightness.
    // Note: You shouldn't be using this in NeoPixel animations.  Call it once.
    //boardLED.setBrightness(100);
    //stripLEDs.setBrightness(100);

    // Initialize the neoPixels to be "black", clearing out any color info
    // stored from before a reset or a crash.
    boardLED.show();
    stripLEDs.show();
    shouldReadTouch = false;
}
void loop() {
    long loopStart = millis();
    
    timeline.update(loopStart);

    long capReading = 0;
    if (shouldReadTouch) {
      capReading = cs_2_3.capacitiveSensor(30);
      Serial.print("Touch value: "); Serial.println(capReading);
    }

    if (timeline.isComplete()) {
      brightness = 0.0;
      shouldReadTouch = true;
    }
    
    // Override brightness if touching
    if (capReading > TOUCH_THRESHOLD) {
      timeline.restartFrom(millis());
      shouldReadTouch = false;
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

void adil(TweenDuino::Timeline &timeline) {
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
      // Dash
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Space
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 0.0));


    // Dash
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

    // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

    // Space
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 0.0));

    // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

    // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

      // Space
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 0.0));

      // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

      // Dash
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

      // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

    // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
}

void speckledCircle(TweenDuino::Timeline &timeline) {
  
  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,1500UL, 100.0));

  // Blip
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 100.0));
  
  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,175, 125.0));

  // Blips
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 125.0));

  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,1500UL, 200.0));

    // Blips
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 200.0));

  timeline.add(*TweenDuino::Tween::to(brightness,755, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,1000, 150.0));

  // Blips
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));;
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 150.0));
  
  timeline.add(*TweenDuino::Tween::to(brightness,1000, 0.0));
}

void waveWithSpecklesInMiddle(TweenDuino::Timeline &timeline) {

  // Set initial bright
  timeline.add(*TweenDuino::Tween::to(brightness,100, 120.0));
  
  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,1000, 200.0));
  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,1000, 120.0));
  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,1000, 250.0));

  // Blips
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 255.0));
    timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 100.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 100.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,100UL, 150.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 250.0));

  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,1000, 120.0));
  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,1000, 200.0));
  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,1000, 120.0));

  // Smooth
  timeline.add(*TweenDuino::Tween::to(brightness,100, 0.0));

}

void makeSOSTimeline(TweenDuino::Timeline &timeline) {
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Dash
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,1000UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  // Dash
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,1000UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  // Dash
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,1000UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  // Dot
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
}
