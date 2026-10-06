#include <stdlib.h>
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "led_strip.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"

#define BOOT_BUTTON_PIN GPIO_NUM_0
#define LED_GPIO_PIN GPIO_NUM_48
#define MAX_LED 1

static const char *TAG = "Hello";
static led_strip_handle_t led_strip;

static void configure_usb(void) {

    const uint8_t hid_report_descriptor[] = {
        0x06, 0xD0, 0xF1, // HID Usage Page
        0x09, 0x01, // HID Usage
        0xA1, 0x01, // HID Collection


    };

}

static void configure_button(void) {

    const gpio_config_t out_conf = {
        .pin_bit_mask = (1ULL << GPIO_NUM_0),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&out_conf);

}

static void configure_led(void) {

    const led_strip_config_t led_conf = {
        .strip_gpio_num = LED_GPIO_PIN,
        .max_leds = MAX_LED,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB
    };

    const led_strip_rmt_config_t rmt_conf = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false
    };

    led_strip_new_rmt_device(&led_conf, &rmt_conf, &led_strip);
    led_strip_clear(led_strip);

}

void app_main(void) {

    configure_button();
    configure_led();

    while (1) {

        int gpio_status = gpio_get_level(BOOT_BUTTON_PIN);

        if (gpio_status == 0) {
            ESP_LOGI(TAG, "Button Held");
            led_strip_set_pixel(led_strip, 0, 0, 25,0);
            led_strip_refresh(led_strip);
        } else {
            ESP_LOGI(TAG, "Button Released");
            led_strip_set_pixel(led_strip, 0, 25, 0,0);
            led_strip_refresh(led_strip);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }

}