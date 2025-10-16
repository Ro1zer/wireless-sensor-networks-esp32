#include <stdlib.h>
#include "wokwi-api.h"
#include <stdio.h>

typedef struct
{
  pin_t rx;
  pin_t tx;
  int last_rx;
} hc12_state_t;

static hc12_state_t *state;

void chip_init(void)
{
  state = (hc12_state_t *)calloc(1, sizeof(hc12_state_t));
  state->rx = pin_init("RX", INPUT);
  state->tx = pin_init("TX", OUTPUT);
  state->last_rx = pin_read(state->rx);
  printf("HC-12 initialized\n");
}

void chip_tick(void)
{
  int value = pin_read(state->rx);
  if (value != state->last_rx)
  {
    if (value == 1)
    {
      printf("HC-12 RX rising edge\n");
      pin_write(state->tx, 1);
    }
    else
    {
      printf("HC-12 RX falling edge\n");
      pin_write(state->tx, 0);
    }
    state->last_rx = value;
  }
}
