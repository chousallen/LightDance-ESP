#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <driver/gpio.h>

#include <led_strip.hpp>

#define LED_STRIP_GPIO_NUM GPIO_NUM_0

uint8_t color_mask[9] = 
{ //g, r, b
    0, 0, 1,
    1, 0, 0,
    0, 0, 0
};

uint8_t pixels[3*3];

void rotate_pixels(uint8_t *pixels, size_t len)
{
    uint8_t temp[3];
    memcpy(temp, &pixels[0], 3);
    memmove(&pixels[0], &pixels[3], (len - 1) * 3);
    memcpy(&pixels[(len - 1) * 3], temp, 3);
}

extern "C" void app_main(void)
{
    LedStrip led_strip(LED_STRIP_GPIO_NUM, 3);
    
    uint8_t bright = 0;
    while (true)
    {
        for (int i = 0; i < 3*3; i++)
        {
            pixels[i] = color_mask[i] * bright;
        }
        
        led_strip.show(pixels);
        bright ++;
        bright %= 256;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    
}