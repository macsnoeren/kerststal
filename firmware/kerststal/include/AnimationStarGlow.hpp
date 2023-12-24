#pragma once
/**
 * @file       : AnimationStarGlow.hpp
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
class AnimationStarGlow: public Animation {
private:
    unsigned int timestamp;
    int8_t v;
    uint8_t a;

public:
    AnimationStarGlow () { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        this->v = -1;
        this->a = 255;
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > 10 ) {
            Hardware::getInstance()->setLightStar(RgbColor(a, a, a));
            Hardware::getInstance()->stripShow();
            this->a = this->a + this->v;
            if ( this->a < 10 ) { this->v = 1; }
            if ( this->a == 255 ) { this->v = -1; }
            this->timestamp = timestamp;
        }
    }   

    void post (unsigned long timestamp) {

    }

    void printName () {
        Serial.print("AnimationStarColorsFlicker");
    }

};

