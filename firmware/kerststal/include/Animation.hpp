/*
Wat is een animatie?
Een animatie zijn opeenvolgende gebeurtenissen, waarbij het tijdstip waarop gebeurtenissen
plaats kunnen vinden anders kunnen zijn.


Ontwerp?
Een idee kan zijn om een gebeurtenis op een bepaalde tijd te laten plaats vinden.
tijd => Status

Deze status kan ook een animatie zijn, bijvoorbeeld knipperen. Dus dan wordt het
tijd => Animatie

Daarnaast kunnen er eventueel ook meerdere Animaties geactiveerd worden, dan krijg je
tijd => [Animaties]


Animator classe bevat een volledige animatie seria wanneer welke animatie start. Meerdere animatie kunnen starten tegelijk.

*/
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
        if ( this->totalActiveAnimation < SIMULTANOUS_ANIMATIONS ) {
            this->activeAnimations[this->totalActiveAnimation] = animation;
        } else {
            Serial.print("addActiveAnimation: Could not add animation due to limited memory!\n");
            return 1;
        }

        return 0;
    }

    void delActiveAnimation (Animation* aninmation) {
        Animation* temp[SIMULTANOUS_ANIMATIONS];
        uint8_t counter = 0;

        for (uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
            temp[i] = NULL; // Initialize in between
            if ( activeAnimations[i] != NULL && activeAnimations[i] != aninmation ) {
                temp[counter] = activeAnimations[i];
                counter++;        
            }
        }

        // Copy the result to the active Animations array
        for (uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
            activeAnimations[i] = temp[i];
        }
    }

    void loop (unsigned long timestamp) {
        if ( this->startTimestamp == 0 ) {
            this->startTimestamp = timestamp;
            this->prevTimestamp = timestamp;
        }

        // Run the active animations
        for ( uint8_t i=0; i < SIMULTANOUS_ANIMATIONS; i++ ) {
            if ( this->activeAnimations[i] != NULL ) {
                this->activeAnimations[i]->loop(timestamp);
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
                
                } else if ( this->events[i].action == STOP ) {
                    this->events[i].animation->post(timestamp);
                    this->delActiveAnimation(this->events[i].animation);

                } else if ( this->events[i].action == RESTART ) {
                    this->startTimestamp = timestamp;
                    this->prevTimestamp = timestamp;
                }
            }
        }

        this->prevTimestamp = timestamp; 
    }

};