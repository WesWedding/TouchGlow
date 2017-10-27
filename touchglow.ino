/**
    Make the white LEDs on a GRBW NeoPixel strip connected to a FLORA
    grow brighter over time.

    And yeah, let's do that to the on-board RGB pixel, too!
*/

#include <TweenDuino.h>
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>
//#include <MemoryFree.h>

#define TOUCH_THRESHOLD 300

// Name some pins.
const int redLED = 7; // On-board red LED.
const int onBoardLEDPin = 8;  // Flora's NeoPixel is on Pin 8.
const int LEDStripPin = 9; // My pixel strip's data line is on pin 9.
const int capSensorPin = 2;
const int capReadPin = 3;

// This is used in loop() to flag whether capacative touching should be checked.
bool shouldReadTouch;

CapacitiveSensor   capSensor = CapacitiveSensor(capReadPin,capSensorPin);        // 10M resistor between pins 2 & 3, pin 2 is sensor pin, add a wire and or foil if desired
Adafruit_NeoPixel boardLED = Adafruit_NeoPixel(1, onBoardLEDPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripLEDs = Adafruit_NeoPixel(7, LEDStripPin, NEO_GRBW);

// This value will be used to set the brightness of LEDs.
float brightness = 0.0;

// This Timeline will be used to modify the brightness value.
TweenDuino::Timeline timeline;

void setup() {

    // Set the Serial output to 9600 baud.
    Serial.begin(9600);

    // Initialize the two LED groups.
    boardLED.begin();
    stripLEDs.begin();


    capSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example

    //Serial.print("startmem=");
    //Serial.println(freeMemory());
    addTweensTo(timeline);
    //Serial.print("aftertl=");
    //Serial.println(freeMemory());

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
    // millis() tells us how much time has passed (in milliseconds) since this sketch started running.
    long loopStart = millis();
    // Tell the Timeline what time it is now.
    timeline.update(loopStart);

    long touchStrength = 0;
    if (shouldReadTouch) {
       // Take 30 samples from the capactive sensor to get a value.
      touchStrength = capSensor.capacitiveSensor(30);
      // Show the result.
      Serial.print("Touch value: "); Serial.println(touchStrength);
    }

    if (timeline.isComplete()) {
      brightness = 0.0;
      shouldReadTouch = true;
    }
    
    // If it seems like we're being touched, restart our timeline from "now"
    // and stop looking for touches.
    if (touchStrength > TOUCH_THRESHOLD) {
      // millis() returns the "now" time in milliseconds since startup.  
      timeline.restartFrom(millis());
      shouldReadTouch = false;
    }

    // 3 colors: Red, Green, Blue whose values range from 0-255.  Some LEDs have a 4th color, like white.
    const uint32_t boardColor = boardLED.Color(0, brightness, 0); // Green.
    const uint32_t stripColor = stripLEDs.Color(0, 0, 0, brightness); // Colors are off, white LED is pure white.

    // Set all the LEDs of the board to "boardColor"
    setStripColors(boardLED, boardColor);
    // Set all of the LEDs of the "strip" to "stripColor"
    setStripColors(stripLEDs, stripColor);

    // This sends the updated pixel color to the hardware.
    stripLEDs.show();
    boardLED.show();
}

// Loop through all of the pixels in a strip and set them to the same color.
void setStripColors(Adafruit_NeoPixel &strip, uint32_t color) {
    const int numPixels = strip.numPixels();

    for (int i=0;i<numPixels;i++) {
        strip.setPixelColor(i, color);
    }
}

// This timeline (memory permitting) will flash the morse-code for the name "Adil"
// if used to control the brightness of a LED.
void addTweensTo(TweenDuino::Timeline &timeline) {
  
  // Dot --------------------------------------------------------
  // Change brightness to 255 over 50 milliseconds. (0.050 seconds)
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  // Change brightness to 0 over 50 milliseconds. (0.050 seconds)
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Dash -------------------------------------------------------   
  // Change brightness to 255 over 50 milliseconds. (0.050 seconds)
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  // Hold the brightness at 255 for 150 milliseconds. (0.150 seconds)
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));
  // Drop the brightness to 0 over 50 milliseconds.(0.050 seconds)
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Space ------------------------------------------------------
  // Hold the brightness at 0 for 150 milliseconds.
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 0.0));


  // Dash -------------------------------------------------------   
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Dot --------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
  
  // Dot --------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Space ------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 0.0));

  // Dot --------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Dot --------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Space ------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 0.0));

  // Dot --------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Dash -------------------------------------------------------   
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,150UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Dot --------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));

  // Dot --------------------------------------------------------
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 255.0));
  timeline.add(*TweenDuino::Tween::to(brightness,50UL, 0.0));
}

