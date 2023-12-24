#pragma once
/**
 * @file       : AnimationLightSlowOn.hpp
 * @description: 
 * @date       : 
 * @author     : Maurice Snoeren (MS)
 * @version    : 0.1 (beta)
 * @license    : GNU version 3.0
 * @todo       : 
 * @updates    :
 * 
 */
#include <stdio.h>
#include <math.h>

#include <Hardware.hpp>
#include <Animation.hpp>

// Abstract Animation class used to implement specific animations.
class AnimationLightFlicker: public Animation {
private:
    unsigned int timestamp;
    uint8_t brightness;
    uint8_t ledNumber;

public:
    AnimationLightFlicker (uint8_t brightness): brightness(brightness) { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        Hardware::getInstance()->setLightLeft(RgbColor(this->brightness, this->brightness, this->brightness));
        Hardware::getInstance()->setLightRight(RgbColor(this->brightness, this->brightness, this->brightness));
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > 100 ) {
            Hardware::getInstance()->setLight(this->ledNumber, RgbColor(this->brightness, this->brightness, this->brightness));
            if ( random(1,10) == 5 ) { // One of the ten we will flicker the light.
                this->ledNumber = random(0, 16);
                Hardware::getInstance()->setLight(this->ledNumber, RgbColor(255, 255, 255));
                //Serial.printf("Flicker: %d\n", this->ledNumber);
            }
            Hardware::getInstance()->stripShow();
            this->timestamp = timestamp;
        }
    }   

    void post (unsigned long timestamp) {
        Hardware::getInstance()->setLightLeft(RgbColor(0, 0, 0));
        Hardware::getInstance()->setLightRight(RgbColor(0, 0, 0));
        Hardware::getInstance()->setLightStar(RgbColor(0, 0, 0));
        Hardware::getInstance()->stripShow();
    }

    void printName () {
        Serial.print("AnimationLightFlicker");
    }

};

