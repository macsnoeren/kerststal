#pragma once
/**
 * @file       : AnimationStarColorsFlicker.hpp
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
class AnimationStarColorsFlicker: public Animation {
private:
    unsigned int timestamp;
    uint8_t a, b, c;
    uint16_t speed;

public:
    AnimationStarColorsFlicker (uint16_t speed=100): speed(speed) { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        this->b = 0;
        this->b = 50;
        this->c = 100;
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > this->speed ) {
            this->timestamp = timestamp;
            a = a + random(1, 5);
            b = b + random(1, 5);
            c = c + random(1, 5);
            Hardware::getInstance()->setLightStar(RgbColor(a, b, c));

            if ( random(1,10) == 5 ) { // One of the ten we will flicker the light.
                uint8_t ledNumber = random(0,8)+16;
                Hardware::getInstance()->setLight(ledNumber, RgbColor(255, 255, 255));
                //Serial.printf("Flicker: %d!!\n", ledNumber);
            }
            Hardware::getInstance()->stripShow();
        }
    }   

    void post (unsigned long timestamp) {

    }

    void printName () {
        Serial.print("AnimationStarColorsFlicker");
    }

};

