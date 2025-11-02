#include <stdlib.h>
#include "wokwi-api.h"
#include <stdio.h>
#include <string.h>

typedef struct
{
  uart_dev_t uart;
  char buffer[64];
  int index;

  float lastTemp;
  float lastHumidity;
  float lastLight;
  int hasAllData;
} hc12_state_t;

static hc12_state_t *state;

void process_line(char *line)
{
  char *token = strtok(line, " ");
  int foundCount = 0;

  while (token != NULL)
  {
    float value;
    char type_char;

    if (sscanf(token, "%c:%f", &type_char, &value) == 2)
    {
      switch (type_char)
      {
      case 'T':
        state->lastTemp = value;
        foundCount++;
        break;
      case 'H':
        state->lastHumidity = value;
        foundCount++;
        break;
      case 'L':
        state->lastLight = value;
        foundCount++;
        break;
      }
    }
    token = strtok(NULL, " ");
  }

  if (foundCount > 0)
  {
    state->hasAllData = 1;
  }

  if (state->hasAllData)
  {
    printf("HC-12: T:%.1f C, H:%.1f %%, L:%.0f lux\n",
           state->lastTemp,
           state->lastHumidity,
           state->lastLight);
  }
}

static void uart_rx_data(void *user_data, uint8_t byte)
{
  if (byte == '\n' || byte == '\r')
  {
    if (state->index > 0)
    {
      state->buffer[state->index] = '\0';
      process_line(state->buffer);
      state->index = 0;
    }
  }
  else
  {
    if (state->index < sizeof(state->buffer) - 1)
    {
      state->buffer[state->index++] = byte;
    }
  }
}

void chip_init(void)
{
  state = (hc12_state_t *)calloc(1, sizeof(hc12_state_t));
  state->lastTemp = 0.0;
  state->lastHumidity = 0.0;
  state->lastLight = 0.0;
  state->hasAllData = 0;

  uart_config_t uart_config = {
      .rx = pin_init("RX", INPUT),
      .tx = pin_init("TX", OUTPUT),
      .baud_rate = 9600,
      .rx_data = uart_rx_data,
      .user_data = state};

  state->uart = uart_init(&uart_config);
  printf("HC-12 initialized\n");
}

void chip_tick(void)
{
  // UART handled in callback
}
