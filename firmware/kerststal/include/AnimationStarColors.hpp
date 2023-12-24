#pragma once
/**
 * @file       : AnimationStarColors.hpp
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
class AnimationStarColors: public Animation {
private:
    unsigned int timestamp;
    uint8_t a, b, c;
    uint16_t speed;

public:
    AnimationStarColors (uint16_t speed=100): speed(speed) { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        this->a = 0;
        this->b = 50;
        this->c = 100;
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > this->speed ) {
            this->timestamp = timestamp;
            this->a = this->a + random(1, 5);
            this->b = this->b + random(1, 5);
            this->c = this->c + random(1, 5);
            Hardware::getInstance()->setLightStar(RgbColor(this->a, this->b, this->c));
            Hardware::getInstance()->stripShow();
        }
    }   

    void post (unsigned long timestamp) {

    }

    void printName () {
        Serial.print("AnimationStarsColors");
    }

};

