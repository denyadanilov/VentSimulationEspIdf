#ifndef SRC_ADAPTER_TIMER_H
#define SRC_ADAPTER_TIMER_H

#include "driver/gptimer.h"
#include <stdint.h>

enum timer_state { DISABLED, ENABLED, IDLE, RUNNING };

typedef struct timer timer_t;
typedef struct timer_builder timer_builder_t;
typedef enum timer_state timer_state_t;

struct timer {
  gptimer_handle_t timer;
  timer_state_t state;
  char name[32];
  void (*enable)(timer_t *);
  void (*disable)(timer_t *);
  void (*start)(timer_t *);
  void (*stop)(timer_t *);
};

struct timer_builder {
  timer_t (*build)(timer_builder_t *);
  gptimer_config_t timer_config;
  gptimer_alarm_config_t alarm_config;
  gptimer_event_callbacks_t event_callbacks;
  uint64_t ticks_per_unit;
  char name[32];
  timer_builder_t *(*with_name)(timer_builder_t *, const char *);
  timer_builder_t *(*with_config_in_milliseconds)(timer_builder_t *);
  timer_builder_t *(*with_config_in_seconds)(timer_builder_t *);
  timer_builder_t *(*with_alarm_on)(timer_builder_t *, uint32_t);
  timer_builder_t *(*with_action)(
      timer_builder_t *,
      bool (*)(struct gptimer_t *timer,
               const gptimer_alarm_event_data_t *event_data, void *arg));
};

timer_builder_t get_timer_builder(void);

#endif // SRC_ADAPTER_TIMER_H