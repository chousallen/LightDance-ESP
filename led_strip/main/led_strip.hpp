#ifndef _LED_STRIP_HPP_
#define _LED_STRIP_HPP_

#include <stdint.h>

#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "pixel_type.h"
#include "led_strip_encoder.h"

class LedStrip
{
    public:
    static const char *TAG;
    const gpio_num_t gpio_num;
    const uint8_t len;
    LedStrip(gpio_num_t _gpio_num, uint8_t _len);
    ~LedStrip();
    void show(const uint8_t *_pixels) const;

    private:
    rmt_channel_handle_t led_chan;
    rmt_encoder_handle_t led_encoder;
    //static const rmt_transmit_config_t tx_config;
};

#endif // _LED_STRIP_HPP_