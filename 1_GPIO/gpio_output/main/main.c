#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void)
{
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(GPIO_NUM_4, 1);
        printf("GPIO HIGH\n");

        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(GPIO_NUM_4, 0);
        printf("GPIO LOW\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}