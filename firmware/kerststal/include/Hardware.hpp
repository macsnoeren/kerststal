#pragma once
/**
 * @file       : Hardware.h
 * @description: Hardware singleton to control the hardware. Based on the LolinS2 (S2 mini).
 *               Microcontroller: https://www.wemos.cc/en/latest/s2/s2_mini.html
 *               Movement sensor: https://www.otronic.nl/nl/pir-sensor-hc-sr501-bewegingssensor.html
 * 
 * @date       : 20-12-2023
 * @author     : Maurice Snoeren (MS)
 * @version    : 1.0
 * @license    : GNU version 3.0
 * @todo       : 
 * @updates    :
 * 
 */
#include <stdio.h>

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <driver/ledc.h>
#include <esp_err.h>

/* Three Neo pixel lights are used with each eight leds. */
#define HARDWARE_PIXEL_COUNT 8*3

/* The data pin of the Neo pixel is connected to 16. */
#define HARDWARE_PIXEL_PIN   GPIO_NUM_16

/* Enumeration to make the control of the Neo pixel easier. */
enum HardwareLightStrip {
    RIGHT_01, RIGHT_02, RIGHT_03, RIGHT_04, RIGHT_05, RIGHT_06, RIGHT_07, RIGHT_08,
    LEFT_01, LEFT_02, LEFT_03, LEFT_04, LEFT_05, LEFT_06, LEFT_07, LEFT_08,
    STAR_01, STAR_02, STAR_03, STAR_04, STAR_05, STAR_06, STAR_07, STAR_08,
};

/* Singleton Hardware class*/
class Hardware {
private:
    NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip;

protected:
    /* Protected constructor in order to create a singleton class. Initialize the class members. */
    Hardware(): strip(HARDWARE_PIXEL_COUNT, HARDWARE_PIXEL_PIN) {
    }

public:
    /* Returns the singleton instance to this class. */
    static Hardware* getInstance() {
        static Hardware _temp;
        return &_temp;
    }

    /* Setup the hardware. */
    uint8_t setup () {
        // Onboard blue led
        gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);
        this->setLed(false);
        delay(2000);
        this->setLed(true);
        delay(2000);
        this->setLed(false);
        

        // Movement sensor to detect human movement
        // https://www.otronic.nl/nl/pir-sensor-hc-sr501-bewegingssensor.html
        // Output pin is connected to GPIO_NUM_8
        gpio_set_direction(GPIO_NUM_8, GPIO_MODE_INPUT);
       
        // Led strip initialization
        strip.Begin();
        strip.Show();

        // Serial communication
        Serial.begin(9600);  
        Serial.print("Kerstal 0.1\n");

        // Setup servo motors using ledc channel 0 - 5
        ledc_timer_config_t ledc_timer = {
            .speed_mode       = LEDC_LOW_SPEED_MODE,
            .timer_num        = LEDC_TIMER_0,
            .freq_hz          = 50,
            .clk_cfg          = LEDC_AUTO_CLK
        };
        ledc_timer.duty_resolution = LEDC_TIMER_13_BIT;
        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

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

        ledc_channel_config_t ledc_channel_1 = {
            .speed_mode     = LEDC_LOW_SPEED_MODE,
            .channel        = LEDC_CHANNEL_1,
            .timer_sel      = LEDC_TIMER_0,
            
            .duty           = 0, // Set duty to 0%
            .hpoint         = 0
        };
        ledc_channel_1.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_1.gpio_num = GPIO_NUM_33;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_1));

        ledc_channel_config_t ledc_channel_2 = {
            .speed_mode     = LEDC_LOW_SPEED_MODE,
            .channel        = LEDC_CHANNEL_2,
            .timer_sel      = LEDC_TIMER_0,
            
            .duty           = 0, // Set duty to 0%
            .hpoint         = 0
        };
        ledc_channel_2.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_2.gpio_num = GPIO_NUM_35;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_2));

        ledc_channel_config_t ledc_channel_3 = {
            .speed_mode     = LEDC_LOW_SPEED_MODE,
            .channel        = LEDC_CHANNEL_3,
            .timer_sel      = LEDC_TIMER_0,
            
            .duty           = 0, // Set duty to 0%
            .hpoint         = 0
        };
        ledc_channel_3.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_3.gpio_num = GPIO_NUM_37;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_3));

        ledc_channel_config_t ledc_channel_4 = {
            .speed_mode     = LEDC_LOW_SPEED_MODE,
            .channel        = LEDC_CHANNEL_4,
            .timer_sel      = LEDC_TIMER_0,
            
            .duty           = 0, // Set duty to 0%
            .hpoint         = 0
        };
        ledc_channel_4.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_4.gpio_num = GPIO_NUM_39;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_4));

        ledc_channel_config_t ledc_channel_5 = {
            .speed_mode     = LEDC_LOW_SPEED_MODE,
            .channel        = LEDC_CHANNEL_5,
            .timer_sel      = LEDC_TIMER_0,
            
            .duty           = 0, // Set duty to 0%
            .hpoint         = 0
        };
        ledc_channel_5.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_5.gpio_num = GPIO_NUM_17;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_5));

        Serial.print("Setup finished!\n");

        return 0;
    }

    // -180 -> 180
    void setServo(ledc_channel_t channel, int16_t degree) {
        uint32_t duty = (degree + 180)*620/360 + 300; 
        //Serial.printf("duty: %lu\n", duty); 
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, channel));
    }

    void setServo0(int16_t degree) {
        setServo(LEDC_CHANNEL_0, degree);
    }

    void setServo1(int16_t degree) {
        setServo(LEDC_CHANNEL_1, degree);
    }

    void setServo2(int16_t degree) {
        setServo(LEDC_CHANNEL_2, degree);
    }

    void setServo3(int16_t degree) {
        setServo(LEDC_CHANNEL_3, degree);
    }

    void setServo4(int16_t degree) {
        setServo(LEDC_CHANNEL_4, degree);
    }

    void setServo5(int16_t degree) {
     setServo(LEDC_CHANNEL_5, degree);
    }

    void setLight(uint16_t index, RgbColor light) {
         strip.SetPixelColor(index, light);
    }

    void setLight(RgbColor light[HARDWARE_PIXEL_COUNT]) {
        for ( uint8_t i=0; i < HARDWARE_PIXEL_COUNT; i++ ) {
            strip.SetPixelColor(i, light[i]);
        }
    }

    void setLightRight(RgbColor light) {
        for ( uint8_t i=0; i < 8; i++ ) {
            strip.SetPixelColor(i, light);
        }
    }

    void setLightLeft(RgbColor light) {
        for ( uint8_t i=8; i < 16; i++ ) {
            strip.SetPixelColor(i, light);
        }
    }

    void setLightStar(RgbColor light) {
        for ( uint8_t i=16; i < 24; i++ ) {
            strip.SetPixelColor(i, light);
        }
    }

    void setLed (bool on) {
        digitalWrite(GPIO_NUM_15, (on ? HIGH : LOW));
    }

    bool isMovementDetected () {
        return digitalRead(GPIO_NUM_8) == HIGH;
    }

    void stripShow () {
        this->strip.Show();
    }

    void loop (unsigned long timestamp) {
        this->setLed(isMovementDetected());
    }

};