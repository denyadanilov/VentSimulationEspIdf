#ifndef SRC_ADAPTER_PIN_H
#define SRC_ADAPTER_PIN_H

#include <stdint.h>

enum pin_mode {
  NONE,
  OUTPUT_MODE,
  INPUT_PULLUP_MODE,
  INPUT_PULLDOWN_MODE,
  INPUT_MODE,
};

enum voltage_state {
  RISING_STATE,
  FALLING_STATE,
  CHANGE_STATE,
  ONLOW_STATE,
  ONHIGH_STATE
};

enum pin_state { LOW_STATE, HIGH_STATE };

typedef struct digital_pin digital_pin_t;
typedef enum pin_state pin_state_t;
typedef enum voltage_state voltage_state_t;
typedef enum pin_mode pin_mode_t;

struct digital_pin {
  uint8_t pin_index;
	void (*set_high)(digital_pin_t *);
	void (*set_low)(digital_pin_t *);
	pin_state_t (*get_state)(digital_pin_t *);
};

digital_pin_t get_digital_pin(uint8_t pin_index, pin_mode_t mode);

#endif // SRC_ADAPTER_PIN_H