#include "fan.h"
#include "adapter/pin.h"

void fan_setup(fan_t* fan);
void fan_turn_on(fan_t* fan);
void fan_turn_off(fan_t* fan);

fan_t create_fan(void) {
    fan_t new_fan;
    new_fan.setup = fan_setup;
    new_fan.turn_on = fan_turn_on;
    new_fan.turn_off = fan_turn_off;
    return new_fan;
}

void fan_setup(fan_t* fan) {
    fan->fan_pin = get_digital_pin(FAN_PIN, OUTPUT_MODE);
}

void fan_turn_on(fan_t * fan) {
    fan->fan_pin.set_high(&fan->fan_pin);
}

void fan_turn_off(fan_t * fan) {
    fan->fan_pin.set_low(&fan->fan_pin);
}