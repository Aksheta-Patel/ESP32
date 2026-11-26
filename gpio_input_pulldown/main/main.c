#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void)
{
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);

    gpio_pulldown_en(GPIO_NUM_4);

    while (1)
    {
        if (gpio_get_level(GPIO_NUM_4) == 1)
        {
            printf("Button Pressed!\n");
        }
        else
        {
            printf("Button Released!\n");
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}