#ifndef FAN_H
#define FAN_H

#define FAN_PIN 16

#include "adapter/pin.h"

typedef struct fan fan_t;

struct fan {
    digital_pin_t fan_pin;
	void (*setup)(fan_t*);
	void (*turn_on)(fan_t*);
	void (*turn_off)(fan_t*);
};

fan_t create_fan(void);

#endif // FAN_H