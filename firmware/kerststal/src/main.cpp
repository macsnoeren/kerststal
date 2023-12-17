/*******************************************************/
/* Project Kerstal                                     */
/* A project for the kerstallenroute in Bavel. The     */
/* ambition is to create an animated kerstal at Bavel. */
/* Just having fun with electronics and clay ;).       */
/*                                                     */
/* Authors are Noortje and Maurice                     */
/*******************************************************/

#include <stdio.h>
#include <Arduino.h>
#include <NeoPixelBus.h>
#include "driver/ledc.h"
#include "esp_err.h"

#include "Hardware.hpp"
#include "Animation.hpp"
#include "AnimationStarColors.hpp"
#include "AnimationStarColorsFlicker.hpp"

AnimationStarColors animationStarColors;
AnimationStarColorsFlicker animationStarsColorsFlicker;

Animator<10, 5> animatorInRest;

void setup() {
    Hardware::getInstance()->setup();

    animatorInRest.addAnimationEvent(0, (Animation*) &animationStarColors, START);
    animatorInRest.addAnimationEvent(10000, (Animation*) &animationStarsColorsFlicker, START);
    animatorInRest.addAnimationEvent(20000, (Animation*) &animationStarColors, STOP);
    animatorInRest.addAnimationEvent(20000, (Animation*) &animationStarsColorsFlicker, STOP);
    animatorInRest.addAnimationEvent(2000, (Animation*) NULL, RESTART);
}

// linksom: 300 -> (stil) 610 => rechtsom 900
uint32_t c = 0;
uint8_t state = 0;
unsigned long ts;
uint8_t a, b, d;

void loop() {
  Hardware *hw = Hardware::getInstance();

  Serial.printf("State %d\n", state);

  if ( state == 0 ) {
    animatorInRest.loop(millis());
    if ( hw->isMovementDetected() ) {
      state = 1;
    }

  } else if ( state == 1 ) {
    ts = millis() / 1000;
    state = 3;
    c = 10;

  } else if ( state == 3 ) {
    hw->setLightLeft(RgbColor(c, c, c));
    hw->setLightRight(RgbColor(c, c, c));
    hw->setLightStar(RgbColor(c, c, c));
    hw->stripShow();
    delay(20);
    c++;
    if ( c == 255 ) {
      state = 4;
    }

  } else if ( state == 4 ) {
    hw->setLightLeft(RgbColor(c, c, c));
    hw->setLightRight(RgbColor(c, c, c));
    //hw->setLightStar(RgbColor(c, c, c));
    hw->stripShow();
    delay(20);
    c--;
    if ( c == 100 ) {
      state = 5;
    }
    if ( millis()/1000 - ts > 10 ) { // After one minute not movement, shut down.
      state = 0;
    }
    if ( hw->isMovementDetected() ) {
      ts = millis() / 1000; // Restart timer!
    }

  } else if ( state == 5 ) {
    hw->setLightLeft(RgbColor(c, c, c));
    hw->setLightRight(RgbColor(c, c, c));
    //setLightStar(RgbColor(c, c, c));
    hw->stripShow();
    delay(20);
    c++;
    if ( c == 255 ) {
      state = 4;
    }
    if ( millis()/1000 - ts > 10 ) { // After one minute not movement, shut down.
      state = 0;
    }
    if ( hw->isMovementDetected() ) {
      ts = millis() / 1000; // Restart timer!
    }
  }
}
