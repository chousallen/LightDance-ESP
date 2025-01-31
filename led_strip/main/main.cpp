#include <cstring>

#include "const.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "pixel_type.h"
#include "led_strip.hpp"

#define LED_STRIP_GPIO_NUM GPIO_NUM_0

uint8_t pixels[3*3] = {
    0, 0, 255,
    0, 0, 0,
    0, 0, 0
};

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
    
    led_strip.show(pixels);
    //rotate_pixels(pixels, 3);
    vTaskDelay(pdMS_TO_TICKS(1000));
    
}