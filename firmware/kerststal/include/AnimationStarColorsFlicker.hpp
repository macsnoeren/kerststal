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

public:
    AnimationStarColorsFlicker () { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        this->b = 0;
        this->b = 50;
        this->c = 100;
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > 100 ) {
            this->timestamp = timestamp;
            a = a + random(1, 5);
            b = b + random(1, 5);
            c = c + random(1, 5);
            Hardware::getInstance()->setLightStar(RgbColor(a, b, c));

            if ( random(1,10) == 5 ) { // One of the ten we will flicker the light.
                Hardware::getInstance()->setLight(random(1,8), RgbColor(255, 255, 255));
            }
            Hardware::getInstance()->stripShow();
        }
    }   

    void post (unsigned long timestamp) {

    }
};

