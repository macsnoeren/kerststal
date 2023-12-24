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
class AnimationLightSlowOn: public Animation {
private:
    unsigned int timestamp;
    uint8_t a;
    uint8_t brightness;

public:
    AnimationLightSlowOn (uint8_t brightness): brightness(brightness) { }

    void pre (unsigned long timestamp) {
        Hardware::getInstance()->setLightLeft(RgbColor(0, 0, 0));
        Hardware::getInstance()->setLightRight(RgbColor(0, 0, 0));
        Hardware::getInstance()->setLightStar(RgbColor(0, 0, 0));
        Hardware::getInstance()->stripShow();

        this->timestamp = timestamp;
        this->a = 0;
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > 50 ) {
            this->timestamp = timestamp;

            if ( this->a != this->brightness ) {
                Hardware::getInstance()->setLightLeft(RgbColor(this->a, this->a, this->a));
                Hardware::getInstance()->setLightRight(RgbColor(this->a, this->a, this->a));
                Hardware::getInstance()->setLightStar(RgbColor(this->a, this->a, this->a));
                Hardware::getInstance()->stripShow();
                this->a++;
            }
        }
    }   

    void post (unsigned long timestamp) {
        
    }

    void printName () {
        Serial.print("AnimationLightSlowOn");
    }

};

