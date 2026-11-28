#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

volatile int event = 0;

void button_isr(void *arg)
{
    event = 1;
}

void app_main(void)
{
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_OUTPUT);

    gpio_set_intr_type(GPIO_NUM_0, GPIO_INTR_ANYEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_0, button_isr, NULL);

    while (1)
    {
        if (event == 1)
        {
            event = 0;

            if (gpio_get_level(GPIO_NUM_0) == 0)
            {
                gpio_set_level(GPIO_NUM_36, 1);
                printf("LED ON\n");
            }
            else
            {
                gpio_set_level(GPIO_NUM_36, 0);
                printf("LED OFF\n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}