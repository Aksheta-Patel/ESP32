#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "INTERNAL_RTC";

// Set ESP32 internal system time
void set_internal_time(void)
{
    struct tm initial_time = {
        .tm_year = 2026 - 1900,
.tm_mon  = 11 - 1,
.tm_mday = 28,
.tm_hour = 13,
.tm_min  = 19,
.tm_sec  = 0,
.tm_isdst = -1
    };

    time_t now = mktime(&initial_time);

    struct timeval tv = {
        .tv_sec = now,
        .tv_usec = 0
    };

    settimeofday(&tv, NULL);
}

// Display current date and time
void print_datetime(void)
{
    time_t now;
    struct tm timeinfo;
    char buffer[64];

    time(&now);

    localtime_r(&now, &timeinfo);

    strftime(buffer, sizeof(buffer),
             "%A, %B %d %Y %H:%M:%S", &timeinfo);

    ESP_LOGI(TAG, "Current date and time: %s", buffer);
}

// FreeRTOS calendar task
void calendar_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Calendar Application Started");

    set_internal_time();

    while (1)
    {
        print_datetime();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing Internal RTC Calendar");

    xTaskCreate(calendar_task, "calendar_task", 8192, NULL, 5, NULL);
}