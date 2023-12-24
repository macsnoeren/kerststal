#pragma once
/**
 * @file       : AnimationJosef.hpp
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
class AnimationJosef: public Animation {
private:
    unsigned int timestamp;
    int8_t v;
    int16_t s;

public:
    AnimationJosef () { }

    void pre (unsigned long timestamp) {
        this->timestamp = timestamp;
        this->v = 2;
        this->s = 0;
        Hardware::getInstance()->setServo0(this->s);
    }
    
    void loop (unsigned long timestamp) {
        if ( timestamp - this->timestamp > 50 ) {
            Hardware::getInstance()->setServo0(this->s);
            this->s = this->s + v;
            
            if ( this->s > 40 ) {
                this->v = -1;
            }

            if ( this->s < -150 ) {
                this->v = 1;
            }

            //Serial.printf("Servo value: %d\n", this->s);
            this->timestamp = timestamp;
        }
    }   

    void post (unsigned long timestamp) {
        this->s = 0;
        Hardware::getInstance()->setServo0(this->s);
    }

    void printName () {
        Serial.print("AnimationJosef");
    }
};

