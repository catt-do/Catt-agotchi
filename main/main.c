#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "buttons.h"
#include "display.h"

#define TAG "Main"

static button_t left   = { .pin = GPIO_NUM_4 };
static button_t right  = { .pin = GPIO_NUM_5 };
static button_t power  = { .pin = GPIO_NUM_6 };
static button_t select = { .pin = GPIO_NUM_7 };

static display_pins_t display_pins = {
    .mosi  = GPIO_NUM_11,
    .sclk  = GPIO_NUM_12,
    .cs    = GPIO_NUM_10,
    .dc    = GPIO_NUM_9,
    .reset = GPIO_NUM_8,
    .bl    = GPIO_NUM_13
};

void app_main(void)
{
    button_init(&left);
    display_init(&display_pins);

    for(;;) {
        if(button_is_pressed(&left)) {
            ESP_LOGI(TAG, "Button pressed");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
