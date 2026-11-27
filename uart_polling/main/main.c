#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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

    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);

    uart_param_config(UART_NUM_0, &config);

    uart_set_pin(UART_NUM_0, TX_PIN, RX_PIN,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    char message[] = "Hello from ESP32\n";
    uart_write_bytes(UART_NUM_0, message, sizeof(message) - 1);

    char data[100];
    int index = 0;
    unsigned char ch;

    while (1)
    {
        int length = uart_read_bytes(
            UART_NUM_0,
            &ch,
            1,
            pdMS_TO_TICKS(100)
        );

        if (length > 0)
        {
            if (ch == '\r' || ch == '\n')
            {
                if (index > 0)
                {
                    data[index] = '\0';

                    uart_write_bytes(UART_NUM_0, "Received: ", 10);
                    uart_write_bytes(UART_NUM_0, data, index);
                    uart_write_bytes(UART_NUM_0, "\r\n", 2);

                    index = 0;
                }
            }
            else if (index < sizeof(data) - 1)
            {
                data[index] = ch;
                index++;
            }
        }
    }
}