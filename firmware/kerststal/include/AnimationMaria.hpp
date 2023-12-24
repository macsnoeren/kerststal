#pragma once
/**
 * @file       : AnimationMaria.hpp
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
class AnimationMaria: public Animation {
private:
    unsigned int timestamp;
    uint8_t state;
    int8_t v;
    int16_t s;

public:
    AnimationMaria () { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        this->v = 2;
        this->s = 0;
        this->state = 0;
        Hardware::getInstance()->setServo1(this->s);
    }
    
    void loop (unsigned long timestamp) {
        if ( this->state == 0 && timestamp - this->timestamp > 1000 ) {
            Hardware::getInstance()->setServo1(-90);
            this->state = 1;
            this->timestamp = timestamp;

        } else if ( this->state == 1 && timestamp - this->timestamp > 8000 ) {
            Hardware::getInstance()->setServo1(0);
            this->state = 2;
            this->timestamp = timestamp;

        } else if ( this->state == 2 && timestamp - this->timestamp > 15000 ) {
            Hardware::getInstance()->setServo1(-90);
            this->state = 1;
            this->timestamp = timestamp;
        }
    }   

    void post (unsigned long timestamp) {
        this->s = 0;
        Hardware::getInstance()->setServo1(this->s);
    }

    void printName () {
        Serial.print("AnimationMaria");
    }
};

