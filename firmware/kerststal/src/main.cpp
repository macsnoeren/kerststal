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
#include "AnimationLightSlowOn.hpp"
#include "AnimationLightFlicker.hpp"

AnimationStarColors animationStarColors;
AnimationStarColorsFlicker animationStarsColorsFlicker;
AnimationLightSlowOn animationLightSlowOn(128);
AnimationLightFlicker animationLightFlicker;

Animator<5, 1> animatorInRest;
Animator<3, 1> animatorMain;

void setup() {
    Hardware::getInstance()->setup();
    animatorInRest.addAnimationEvent(0, (Animation*) &animationStarColors, START);
    animatorInRest.addAnimationEvent(20000, (Animation*) &animationStarColors, STOP);
    animatorInRest.addAnimationEvent(20000, (Animation*) &animationStarsColorsFlicker, START);
    animatorInRest.addAnimationEvent(100000, (Animation*) &animationStarsColorsFlicker, STOP);
    animatorInRest.addAnimationEvent(100000, (Animation*) NULL, RESTART);

    animatorMain.addAnimationEvent(0, (Animation*) &animationLightSlowOn, START);
    animatorMain.addAnimationEvent(10000, (Animation*) &animationLightSlowOn, STOP);
    animatorMain.addAnimationEvent(10000, (Animation*) &animationLightFlicker, START);
}

uint8_t state = 0;
unsigned long ts = 0;
void loop() {
  Hardware *hw = Hardware::getInstance();

  //Serial.printf("State %d\n", state);

  if ( state == 0 ) {
    animatorInRest.loop(millis());
    if ( hw->isMovementDetected() ) {
      state = 1;
      ts = millis()/1000; // Init the timestamp
    }

  } else if ( state == 1 ) {
    animatorMain.loop(millis());

    if ( millis()/1000 - ts > 10 ) { // After one minute not movement, shut down.
      state = 0;
    }

    if ( hw->isMovementDetected() ) {
      ts = millis() / 1000; // Restart timer!
    }
  } 
}
