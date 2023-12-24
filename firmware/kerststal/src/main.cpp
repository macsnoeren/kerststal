/*******************************************************/
/* Project Kerstal                                     */
/* A project for the kerstallenroute in Bavel. The     */
/* ambition is to create an animated kerstal at Bavel. */
/* Just having fun with electronics and clay ;).       */
/*                                                     */
/* Authors are Noortje and Maurice                     */
/* Idea: Second animation dancing on music.            */
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
#include "AnimationJosef.hpp"
#include "AnimationMaria.hpp"
#include "AnimationStarGlow.hpp"

AnimationStarColors animationStarColors;
AnimationStarColors animationStarColors10(10);
AnimationStarColorsFlicker animationStarsColorsFlicker;
AnimationLightSlowOn animationLightSlowOn(50);
AnimationLightFlicker animationLightFlicker(50);
AnimationJosef animationJosef;
AnimationStarGlow animationStarGlow;
AnimationMaria animationMaria;

Animator<1, 1> animatorInRest;
Animator<15, 10> animatorMain;

void setup() {
    Hardware::getInstance()->setup();
    animatorInRest.addAnimationEvent(0, (Animation*) &animationStarsColorsFlicker, START);

    animatorMain.addAnimationEvent(0, (Animation*) &animationLightSlowOn, START);
    animatorMain.addAnimationEvent(5000, (Animation*) &animationLightSlowOn, STOP);
    animatorMain.addAnimationEvent(5000, (Animation*) &animationJosef, START);
    animatorMain.addAnimationEvent(5000, (Animation*) &animationMaria, START);
    animatorMain.addAnimationEvent(5000, (Animation*) &animationLightFlicker, START);
    animatorMain.addAnimationEvent(5000, (Animation*) &animationStarGlow, START);    
    animatorMain.addAnimationEvent(10000, (Animation*) &animationStarGlow, STOP);    
    animatorMain.addAnimationEvent(10000, (Animation*) &animationStarColors10, START);        
    animatorMain.addAnimationEvent(20000, (Animation*) &animationStarGlow, START);    
    animatorMain.addAnimationEvent(20000, (Animation*) &animationStarColors10, STOP);        
    animatorMain.addAnimationEvent(30000, (Animation*) &animationStarGlow, START);    
    animatorMain.addAnimationEvent(30000, (Animation*) &animationStarColors10, STOP);        

    Hardware::getInstance()->setLightLeft(RgbColor(0, 0, 0));
    Hardware::getInstance()->setLightRight(RgbColor(0, 0, 0));
    Hardware::getInstance()->setLightStar(RgbColor(0, 0, 0));
    Hardware::getInstance()->stripShow();

    delay(1000);
    Serial.print("Setup done!\n");
}

uint8_t state = 1;
unsigned long ts = millis()/1000;

void loop() {
  Hardware *hw = Hardware::getInstance();
  hw->loop(millis());

  //Serial.printf("State %d\n", state);

  if (state == 1 ) {
    animatorInRest.loop(millis());
    if ( hw->isMovementDetected() ) { // Wait 10 seconds before starting the application
    Serial.print("Movement detected.\n");
      state = 2;
      animatorInRest.restart();
      ts = millis()/1000; // Init the timestamp
    }

  } else if ( state == 2 ) {
    animatorMain.loop(millis());

    if ( millis()/1000 - ts > 10 ) { // After one minute not movement, shut down.
        state = 1;
        Hardware::getInstance()->setLightLeft(RgbColor(0, 0, 0));
        Hardware::getInstance()->setLightRight(RgbColor(0, 0, 0));
        Hardware::getInstance()->setLightStar(RgbColor(0, 0, 0));
        Hardware::getInstance()->stripShow();
        animatorMain.restart();
    }

    if ( hw->isMovementDetected() ) {
      ts = millis() / 1000; // Restart timer!
    }
  } 
}
