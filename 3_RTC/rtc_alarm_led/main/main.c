#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

#include "esp_timer.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2

#define ALARM_HOUR 17
#define ALARM_MINUTE 28

static const char *TAG = "RTC_ALARM";

static int last_alarm_day = -1;

esp_timer_handle_t alarm_timer;
esp_timer_handle_t led_timer;

// Initialize LED
void init_led(void)
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);
}

// Turn OFF LED after 5 seconds
void led_off_callback(void *arg)
{
    gpio_set_level(LED_PIN, 0);
    ESP_LOGI(TAG, "LED OFF");
}

// Check alarm time
bool check_alarm_time(struct tm *timeinfo)
{
    return (timeinfo->tm_hour == ALARM_HOUR &&
            timeinfo->tm_min == ALARM_MINUTE);
}

// Timer callback
void periodic_timer_callback(void *arg)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    ESP_LOGI(TAG, "Current time: %02d:%02d:%02d",
             timeinfo.tm_hour,
             timeinfo.tm_min,
             timeinfo.tm_sec);

    if (check_alarm_time(&timeinfo) &&
        last_alarm_day != timeinfo.tm_yday)
    {
        last_alarm_day = timeinfo.tm_yday;

        ESP_LOGI(TAG, "Alarm triggered!");

        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "LED ON");

        esp_timer_start_once(led_timer, 5000000);
    }
}

// Set internal system time
void set_internal_time(struct tm *timeinfo)
{
    time_t now = mktime(timeinfo);

    struct timeval tv = {
        .tv_sec = now,
        .tv_usec = 0
    };

    settimeofday(&tv, NULL);
}

void app_main(void)
{
    init_led();

    // Set initial time
    struct tm initial_time = {
        .tm_year = 2026 - 1900,
        .tm_mon = 11 - 1,
        .tm_mday = 28,
        .tm_hour = 17,
        .tm_min = 24,
        .tm_sec = 0,
        .tm_isdst = -1
    };

    set_internal_time(&initial_time);

    // Create LED timer
    const esp_timer_create_args_t led_timer_args = {
        .callback = &led_off_callback,
        .name = "led_timer"
    };

    esp_timer_create(&led_timer_args, &led_timer);

    // Create periodic alarm timer
    const esp_timer_create_args_t alarm_timer_args = {
        .callback = &periodic_timer_callback,
        .name = "alarm_timer"
    };

    esp_timer_create(&alarm_timer_args, &alarm_timer);

    // Check alarm every 5 seconds
    esp_timer_start_periodic(alarm_timer, 5000000);

    ESP_LOGI(TAG, "RTC Alarm Application Started");
}