#include <stdlib.h>
#include "wokwi-api.h"
#include <stdio.h>

typedef struct
{
  pin_t rx;
  pin_t tx;
  uart_dev_t uart;
} hc12_state_t;

static void uart_rx_data(void *user_data, uint8_t byte)
{
  // Обробляємо отриманий байт
  printf("HC-12 RX: %c\n", byte);
}

void chip_init(void)
{
  hc12_state_t *state = (hc12_state_t *)calloc(1, sizeof(hc12_state_t));
  
  // Ініціалізуємо UART замість ручної перевірки пінів
  uart_config_t uart_config = {
    .rx = pin_init("RX", INPUT),
    .tx = pin_init("TX", OUTPUT),
    .baud_rate = 9600,
    .rx_data = uart_rx_data,
    .user_data = state
  };
  state->uart = uart_init(&uart_config);
  
  printf("HC-12 initialized (UART mode)\n");
}