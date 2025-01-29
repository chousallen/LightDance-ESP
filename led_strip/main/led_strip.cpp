#include "led_strip.hpp"

#include <stdint.h>

#include "const.h"
#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "pixel_type.h"
#include "led_strip_encoder.h"

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define DEFAULT_MEM_BLOCK_SYMBOLS 64
#define DEFAULT_TRANS_QUEUE_DEPTH 4

const char *LedStrip::TAG = "led_strip";

LedStrip::LedStrip(gpio_num_t _gpio_num, uint8_t _len) : gpio_num(_gpio_num), len(_len)
{
    ESP_LOGD(TAG, "Constructor");
    rmt_channel_handle_t led_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config = 
    {
        .gpio_num = gpio_num,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .mem_block_symbols = DEFAULT_MEM_BLOCK_SYMBOLS,
        .trans_queue_depth = DEFAULT_TRANS_QUEUE_DEPTH, 
        .intr_priority = 0,
        .flags = {
            .invert_out = 0,
            .with_dma = 0,
            .io_loop_back = 0,
            .io_od_mode = 1,
        }
    };
    
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));
    ESP_ERROR_CHECK(rmt_enable(led_chan));
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));
}

LedStrip::~LedStrip()
{
    ESP_LOGD(TAG, "Destructor");
    rmt_disable(led_chan);
    rmt_del_channel(led_chan);
}

void LedStrip::show(const uint8_t *_pixels) const
{
    ESP_LOGD(TAG, "Show pixels");
    rmt_transmit_config_t tx_config = 
    {
        .loop_count = 0, // no transfer loop
        .flags = {
            .eot_level = 1, // set the output level for the "End Of Transmission"
            .queue_nonblocking = 1, // If set, when the transaction queue is full, driver will not block the thread but return directly
        }
    };
    ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, _pixels, 3 * len * sizeof(uint8_t), &tx_config));
}
