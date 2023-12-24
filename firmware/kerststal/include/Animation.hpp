#pragma once
/**
 * @file       : Animation.hpp
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

#include <Hardware.hpp>

// Abstract Animation class used to implement specific animations.
class Animation {
public:
    Animation () { }

    virtual void pre (unsigned long timestamp) = 0;
    virtual void loop (unsigned long timestamp) = 0;
    virtual void post (unsigned long timestamp) = 0;
    virtual void printName () = 0;
};

enum AnimationAction {
    NONE, START, STOP, RESTART
};

struct AnimationEvent {
    unsigned long timestamp;
    Animation* animation;
    AnimationAction action;
};

template<uint8_t TOTAL_EVENTS, uint8_t SIMULTANOUS_ANIMATIONS>
class Animator {
private:
    AnimationEvent events[TOTAL_EVENTS];
    Animation* activeAnimations[SIMULTANOUS_ANIMATIONS];

    uint8_t totalEvents;
    uint8_t totalActiveAnimation;

    unsigned int startTimestamp;
    unsigned int prevTimestamp;

public:
    Animator (): totalEvents(0), totalActiveAnimation(0), startTimestamp(0), prevTimestamp(0) {
        // Initialize the events
        for (uint8_t i=0; i < TOTAL_EVENTS; i++ ) {
            AnimationEvent* event = &this->events[i];
            event->timestamp = 0;
            event->animation = NULL;
            event->action = NONE;
        }

        // Initialize the animation
        for (uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
            activeAnimations[i] = NULL;
        }
    }

    uint8_t addAnimationEvent (unsigned long timestamp, Animation* animation, AnimationAction action) {
        if ( this->totalEvents < TOTAL_EVENTS ) {
            this->events[this->totalEvents].timestamp = timestamp;
            this->events[this->totalEvents].animation = animation;
            this->events[this->totalEvents].action = action;
            this->totalEvents++;

        } else {
            Serial.print("addAnimationEvent: Could not add event due to limited memory!\n");
            return 1;
        }

        return 0;
    }

    uint8_t addActiveAnimation (Animation* animation) {
        bool animationFound = false;
        for ( uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
            if ( this->activeAnimations[i] == animation ) {
                animationFound = true;
                Serial.printf("addActiveAnimation: Animation already found on position %d.\n", i);
            }
        }

        if ( !animationFound ) {
            if ( this->totalActiveAnimation < SIMULTANOUS_ANIMATIONS ) {
                this->activeAnimations[this->totalActiveAnimation] = animation;
                this->totalActiveAnimation++;
                Serial.printf("Added active animation (%d): ", this->totalActiveAnimation);
                animation->printName(); Serial.print("\n");

            } else {
                Serial.print("addActiveAnimation: Could not add animation due to limited memory!\n");
                return 1;
            }
        }

        return 0;
    }

    void delActiveAnimation (Animation* aninmation) {
        bool animationFound = false;
        for ( uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
            if ( this->activeAnimations[i] == aninmation ) {
                animationFound = true;
                Serial.printf("Animation found on position %d to delete\n", i);
            }
        }

        if ( animationFound ) {
            Animation* temp[SIMULTANOUS_ANIMATIONS];
            uint8_t counter = 0;

            for (uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
                temp[i] = NULL; // Initialize in between
                if ( this->activeAnimations[i] != NULL && this->activeAnimations[i] != aninmation ) {
                    temp[counter] = activeAnimations[i];
                    counter++;        
                }
            }

            // Copy the result to the active Animations array
            for (uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
                activeAnimations[i] = temp[i];
            }
            this->totalActiveAnimation--;
            Serial.printf("Deleted active animation (%d): ", this->totalActiveAnimation);
            aninmation->printName(); Serial.print("\n");
        }
    }

    void loop (unsigned long timestamp) {
        if ( this->startTimestamp == 0 ) {
            this->startTimestamp = timestamp;
            this->prevTimestamp = timestamp;
            Serial.print("Starting a fresh new animation\n");
        }

        // Run the active animations
        for ( uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
            if ( this->activeAnimations[i] != NULL ) {
                this->activeAnimations[i]->loop(timestamp);
                //Serial.printf("Loop animation: %d\n", i);
            }
        }

        // Check if it is required to change animation
        for ( uint8_t i=0; i < TOTAL_EVENTS; i++ ) {
            if ( this->events[i].timestamp + this->startTimestamp >= this->prevTimestamp && 
                 this->events[i].timestamp + this->startTimestamp < timestamp) {
                // Execute event, timestamp is found!
                if ( this->events[i].action == START ) {
                    this->events[i].animation->pre(timestamp);
                    this->addActiveAnimation(this->events[i].animation);
                    Serial.printf("Add active animation %d.\n", i);
                
                } else if ( this->events[i].action == STOP ) {
                    this->events[i].animation->post(timestamp);
                    this->delActiveAnimation(this->events[i].animation);
                    Serial.printf("Del active animation %d.\n", i);

                } else if ( this->events[i].action == RESTART ) {
                    this->restart();
                    Serial.printf("Restart active animation %d.\n", i);
                }
            }
        }

        this->prevTimestamp = timestamp; 
    }

    void restart () {
        this->startTimestamp = 0;
        this->totalActiveAnimation = 0;
        for (uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++) {
            if ( this->activeAnimations[i] != NULL ) { // Required to do a post of all active animations!
                this->activeAnimations[i]->post(millis());
            }
            this->activeAnimations[i] = NULL;
        }
    }

};