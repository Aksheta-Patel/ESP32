#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void)
{
    // Configure BOOT button as INPUT
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);

    while (1)
    {
        if (gpio_get_level(GPIO_NUM_0) == 0)
        {
            printf("Button Pressed\n");
        }
        else
        {
            printf("Button Released\n");
        }

       vTaskDelay(pdMS_TO_TICKS(1000));
    }
}