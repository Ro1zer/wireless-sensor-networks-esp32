#include <stdlib.h>
#include "wokwi-api.h"
#include <stdio.h>
#include <string.h>

typedef struct {
  uart_dev_t uart;
  char buffer[64];       // buffer for incoming line
  int index;             // current buffer index
  // Тут можна додати змінні для зберігання останніх значень, якщо потрібно
} hc12_state_t;

static hc12_state_t *state;

// Parse line and identify values robustly
void process_line(char *line) {
    // Розділяємо рядок на окремі токени, використовуючи пробіл як роздільник
    char *token = strtok(line, " ");
    while (token != NULL) {
        float value;
        char type_char;
        
        // Намагаємося розібрати кожен токен у форматі "X:Value"
        if (sscanf(token, "%c:%f", &type_char, &value) == 2) {
            switch (type_char) {
                case 'T':
                    printf("HC-12: T: %.1f C\n", value);
                    break;
                case 'H':
                    printf("HC-12: H: %.1f %%\n", value);
                    break;
                case 'L':
                    // Для яскравості, яка є цілим числом
                    printf("HC-12: L: %.0f lux\n", value);
                    break;
                default:
                    printf("HC-12: Unknowk type of data '%c' with value %.2f\n", type_char, value);
                    break;
            }
        } else {
            // Якщо токен не відповідає очікуваному формату
            // printf("HC-12: Не вдалося розібрати токен: %s\n", token);
        }
        
        // Переходимо до наступного токена
        token = strtok(NULL, " ");
    }
}

// UART callback: read characters and form full lines
static void uart_rx_data(void *user_data, uint8_t byte) {
  if (byte == '\n' || byte == '\r') {
    if (state->index > 0) {
      state->buffer[state->index] = '\0';
      process_line(state->buffer);
      state->index = 0;
    }
  } else {
    if (state->index < sizeof(state->buffer) - 1) {
      state->buffer[state->index++] = byte;
    }
  }
}

void chip_init(void) {
  state = (hc12_state_t *)calloc(1, sizeof(hc12_state_t));

  uart_config_t uart_config = {
    .rx = pin_init("RX", INPUT),
    .tx = pin_init("TX", OUTPUT),
    .baud_rate = 9600,
    .rx_data = uart_rx_data,
    .user_data = state
  };

  state->uart = uart_init(&uart_config);
  printf("HC-12 initialized\n");
}

void chip_tick(void) {
  // UART handled in callback
}
