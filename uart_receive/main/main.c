#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "my_uart";

static void receive_task(void *arg)
{
    uart_config_t config =
    {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &config));

    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, GPIO_NUM_43, GPIO_NUM_44,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    const int buffer_size = 2048;

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, buffer_size, buffer_size,
                                        0, NULL, 0));

    uint8_t buffer[128];
    uint8_t ch;
    int index = 0;

    while (1)
    {
        int len = uart_read_bytes(UART_NUM_0, &ch, 1, pdMS_TO_TICKS(100));

        if (len > 0)
        {
            if (ch == '\r' || ch == '\n')
            {
                if (index > 0)
                {
                    buffer[index] = '\0';

                    
printf("Received Data: %s\n", (char *)buffer);


                    

                    index = 0;
                }
            }
            else if (index < sizeof(buffer) - 1)
            {
                buffer[index] = ch;
                index++;
            }
        }
    }
}

void app_main(void)
{
    xTaskCreate(receive_task, "receive_task", 4096, NULL, 10, NULL);
}