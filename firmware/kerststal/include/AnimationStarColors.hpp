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

public:
    AnimationStarColors () { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        this->b = 0;
        this->b = 50;
        this->c = 100;
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > 100 ) {
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
};

