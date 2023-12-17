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

public:
    AnimationLightSlowOn () { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        this->a = 0;
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > 100 ) {
            this->timestamp = timestamp;

            if ( a != 255 ) {
                Hardware::getInstance()->setLightLeft(RgbColor(a, a, a));
                Hardware::getInstance()->setLightRight(RgbColor(a, a, a));
                Hardware::getInstance()->stripShow();
                this->a++;
            }
        }
    }   

    void post (unsigned long timestamp) {

    }
};

