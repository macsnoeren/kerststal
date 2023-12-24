#pragma once
/**
 * @file       : ServoController.hpp
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

// Abstract Animation class used to implement specific animations.
class ServoController {
private:
    int16_t setpoint;
    int16_t output;

public:
    ServoController () { 
        ledc_channel_config_t ledc_channel_0 = {
            .speed_mode     = LEDC_LOW_SPEED_MODE,
            .channel        = LEDC_CHANNEL_0,
            .timer_sel      = LEDC_TIMER_0,
            
            .duty           = 0, // Set duty to 0%
            .hpoint         = 0
        };
        ledc_channel_0.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_0.gpio_num = GPIO_NUM_18;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_0));

    }

};

