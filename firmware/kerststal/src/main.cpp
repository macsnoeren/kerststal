#include <stdio.h>
#include <Arduino.h>
#include <NeoPixelBus.h>
#include "driver/ledc.h"
#include "esp_err.h"

// https://www.otronic.nl/nl/pir-sensor-hc-sr501-bewegingssensor.html
// HC-SR04 Distance work only with 5V. Although it works ok, it is required to protect the input port with a res. to get 3.3V 

const uint16_t PixelCount = 8*3; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 16;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128//10//128
#define ledDelay 500

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
  digitalWrite(GPIO_NUM_4, LOW);
  
  gpio_set_direction(GPIO_NUM_8, GPIO_MODE_INPUT);
  //gpio_pullup_en(GPIO_NUM_8);

  strip.Begin();
  strip.Show();

  Serial.begin(9600);  
  Serial.print("hello\n");

  // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 50,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer.duty_resolution = LEDC_TIMER_13_BIT;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
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

    // Prepare and then apply the LEDC PWM channel configuration
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

    // Prepare and then apply the LEDC PWM channel configuration
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

    // Prepare and then apply the LEDC PWM channel configuration
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

    // Prepare and then apply the LEDC PWM channel configuration
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

    // Prepare and then apply the LEDC PWM channel configuration
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

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel_6 = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_6,
        .timer_sel      = LEDC_TIMER_0,
        
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_6.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_6.gpio_num = GPIO_NUM_21;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_6));

    Serial.print("Start");

}

float getDistance () {
  float duration; 
  digitalWrite(GPIO_NUM_4, LOW);  
	delayMicroseconds(2);  
	digitalWrite(GPIO_NUM_4, HIGH);  
  delayMicroseconds(10);  
	digitalWrite(GPIO_NUM_4, LOW);
  duration = pulseIn(GPIO_NUM_18, HIGH);
  return (duration*.0343)/2;  
}


// -180 -> 180
void setServo(ledc_channel_t channel, int16_t degree) {
  uint32_t duty = (degree + 180)*620/360 + 300; 
  Serial.printf("duty: %lu\n", duty); 
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


/*
void setServo6(int16_t degree) {
  setServo(LEDC_CHANNEL_6, degree);
}
*/

bool isMovementDetected () {
  return digitalRead(GPIO_NUM_8) == HIGH;
}

// linksom: 300 -> (stil) 610 => rechtsom 900
uint32_t c = 0;
uint8_t state = 0;
unsigned long ts;
uint8_t a, b, d;


void loop() {
  Serial.printf("State %d\n", state);

  if ( state == 0 ) {
    setLightLeft(RgbColor(10, 10, 10));
    setLightRight(RgbColor(10, 10, 10));
    setLightStar(RgbColor(10, 10, 10));
    strip.Show();
    state = 1;
    a = 0;
    b = 50;
    d = 100;

  } else if ( state == 1) {
    a = a + random(1, 5);
    b = b + random(1, 5);
    d = d + random(1, 5);
    setLightStar(RgbColor(a, b, d));
    strip.Show();
    delay(100);
    if ( isMovementDetected() ) {
      state = 2;
    }

  } else if ( state == 2 ) {
    ts = millis() / 1000;
    state = 3;
    c = 10;

  } else if ( state == 3 ) {
    setLightLeft(RgbColor(c, c, c));
    setLightRight(RgbColor(c, c, c));
    setLightStar(RgbColor(c, c, c));
    strip.Show();
    delay(20);
    c++;
    if ( c == 255 ) {
      state = 4;
    }

  } else if ( state == 4 ) {
    setLightLeft(RgbColor(c, c, c));
    setLightRight(RgbColor(c, c, c));
    //setLightStar(RgbColor(c, c, c));
    strip.Show();
    delay(20);
    c--;
    if ( c == 100 ) {
      state = 5;
    }
    if ( millis()/1000 - ts > 10 ) { // After one minute not movement, shut down.
      state = 0;
    }
    if ( isMovementDetected() ) {
      ts = millis() / 1000; // Restart timer!
    }

  } else if ( state == 5 ) {
    setLightLeft(RgbColor(c, c, c));
    setLightRight(RgbColor(c, c, c));
    //setLightStar(RgbColor(c, c, c));
    strip.Show();
    delay(20);
    c++;
    if ( c == 255 ) {
      state = 4;
    }
    if ( millis()/1000 - ts > 10 ) { // After one minute not movement, shut down.
      state = 0;
    }
    if ( isMovementDetected() ) {
      ts = millis() / 1000; // Restart timer!
    }
  }
}
