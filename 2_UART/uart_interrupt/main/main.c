#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define TX_PIN GPIO_NUM_43
#define RX_PIN GPIO_NUM_44

void app_main(void)
{
    uart_config_t config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    QueueHandle_t uart_queue;
    uart_event_t event;
    char data[100];

    uart_driver_install(UART_NUM_0, 1024, 1024, 10, &uart_queue, 0);

    uart_param_config(UART_NUM_0, &config);

    uart_set_pin(UART_NUM_0, TX_PIN, RX_PIN,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_write_bytes(UART_NUM_0, "Hello from ESP32\r\n", 18);

    while (1)
    {
        if (xQueueReceive(uart_queue, &event, portMAX_DELAY))
        {
            if (event.type == UART_DATA)
            {
                int length = uart_read_bytes(
                    UART_NUM_0, data, sizeof(data), 0
                );

                if (length > 0)
                {
                    uart_write_bytes(UART_NUM_0, "Received: ", 10);
                    uart_write_bytes(UART_NUM_0, data, length);
                }
            }
        }
    }
}