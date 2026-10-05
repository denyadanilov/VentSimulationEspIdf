#include "adapter/pin.h"
#include "driver/gpio.h"

void set_high(digital_pin_t *pin);
void set_low(digital_pin_t *pin);
pin_state_t get_state(digital_pin_t *pin);
uint32_t pin_state_to_esp(pin_state_t state);
gpio_mode_t pin_mode_to_esp(pin_mode_t pin_mode);
gpio_pull_mode_t pull_mode_to_esp(pin_mode_t pin_mode);

digital_pin_t get_digital_pin(uint8_t pin_index, pin_mode_t mode) {
  gpio_reset_pin(pin_index);
  gpio_set_direction(pin_index, pin_mode_to_esp(mode));
  gpio_set_pull_mode(pin_index, pull_mode_to_esp(mode));

    digital_pin_t pin;
    pin.pin_index = pin_index;
    pin.set_high = set_high;
    pin.set_low = set_low;
    pin.get_state = get_state;
    return pin;
}

void set_high(digital_pin_t *pin) {
	uint32_t state = pin_state_to_esp(HIGH_STATE);
    gpio_set_level(pin->pin_index, state);
}

void set_low(digital_pin_t *pin) {
    gpio_set_level(pin->pin_index, pin_state_to_esp(LOW_STATE));
}

pin_state_t get_state(digital_pin_t *pin) {
    return gpio_get_level(pin->pin_index) ? HIGH_STATE : LOW_STATE;
}

uint32_t pin_state_to_esp(pin_state_t state) {
  switch (state) {
  case HIGH_STATE:
    return 1;
  case LOW_STATE:
    return 0;
  default:
    return 0;
  }
}

gpio_mode_t pin_mode_to_esp(pin_mode_t pin_mode) {
  switch (pin_mode) {
  case OUTPUT_MODE:
    return GPIO_MODE_OUTPUT;
  case INPUT_PULLUP_MODE:
  case INPUT_PULLDOWN_MODE:
    return GPIO_MODE_INPUT;
  default:
    return GPIO_MODE_INPUT;
  }
}

gpio_pull_mode_t pull_mode_to_esp(pin_mode_t pin_mode) {
  switch (pin_mode) {
  case INPUT_PULLUP_MODE:
    return GPIO_PULLUP_ONLY;
  case INPUT_PULLDOWN_MODE:
    return GPIO_PULLDOWN_ONLY;
  default:
    return GPIO_FLOATING;
  }
}