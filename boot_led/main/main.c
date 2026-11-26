#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void)
{
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_OUTPUT);

    while (1)
    {
        if (gpio_get_level(GPIO_NUM_0) == 0)
        {
            gpio_set_level(GPIO_NUM_36, 1);
            printf("led is on\n");
        }
        else
        {
            gpio_set_level(GPIO_NUM_36, 0);
            printf("led is off\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}