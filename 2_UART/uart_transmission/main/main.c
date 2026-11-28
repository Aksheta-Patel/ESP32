#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "my_uart";

static void transmit_task(void *arg)
{
    const uart_config_t config =
    {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &config));

    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, GPIO_NUM_1, GPIO_NUM_3,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 2048, 2048, 0, NULL, 0));

    const char *data = "Hello from ESP32 via UART!\r\n";

    while (1)
    {
        uart_write_bytes(UART_NUM_0, data, strlen(data));

        ESP_LOGI(TAG, "UART Data Sent: %s", data);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    xTaskCreate(transmit_task, "transmit_task", 2048, NULL, 10, NULL);
}