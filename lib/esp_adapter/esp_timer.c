#include "adapter/timer.h"
#include <stdint.h>
#include "driver/gptimer.h"
#include "adapter/logger.h"

static const char *state_names[] = {
    "DISABLED",
    "ENABLED",
    "IDLE",
    "RUNNING"
};

static timer_builder_t *with_name(timer_builder_t *builder, const char *name);
static timer_builder_t *with_config_in_milliseconds(timer_builder_t *builder);
static timer_builder_t *with_config_in_seconds(timer_builder_t *builder);
static timer_builder_t *with_alarm_on(timer_builder_t *builder, uint32_t alarm);
static timer_builder_t *with_action(timer_builder_t *builder, bool (*action)(struct gptimer_t *timer, const gptimer_alarm_event_data_t *event_data, void *arg));
static timer_t build(timer_builder_t *builder);
static void enable(timer_t *timer);
static void disable(timer_t *timer);
static void start(timer_t *timer);
static void stop(timer_t *timer);
static const char *get_state_name(timer_state_t state);

timer_builder_t get_timer_builder(void) {
  timer_builder_t builder = {0};
  builder.with_config_in_milliseconds = with_config_in_milliseconds;
  builder.with_config_in_seconds = with_config_in_seconds;
  builder.with_alarm_on = with_alarm_on;
  builder.with_name = with_name;
  builder.with_action = with_action;
  builder.build = build;
  return builder;
}

static timer_builder_t *with_config_in_milliseconds(timer_builder_t *builder) {
	gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000000,
      .intr_priority = 0,
  };

	builder->timer_config = timer_config;
  builder->ticks_per_unit = 1000;

  return builder;
}

static timer_builder_t *with_config_in_seconds(timer_builder_t *builder) {
	gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000000,
      .intr_priority = 0,
  };

	builder->timer_config = timer_config;
  builder->ticks_per_unit = 1000000;

  return builder;
}

static timer_builder_t *with_alarm_on(timer_builder_t *builder, uint32_t alarm) {
  	gptimer_alarm_config_t alarm_config = {
    .reload_count = 0,
    .alarm_count = (uint64_t)alarm * builder->ticks_per_unit,
    .flags.auto_reload_on_alarm = true,
	};

	builder->alarm_config = alarm_config;

  return builder;
}

static timer_builder_t *with_action(timer_builder_t *builder, bool (*action)(struct gptimer_t *timer, const gptimer_alarm_event_data_t *event_data, void *arg)) {
  gptimer_event_callbacks_t cbs = {
    .on_alarm = action,
  };
  builder->event_callbacks = cbs;
  return builder;
}

static timer_builder_t *with_name(timer_builder_t *builder, const char *name) {
  snprintf(builder->name, sizeof(builder->name), "%s", name);
  return builder;
}

static timer_t build(timer_builder_t *builder) {
	gptimer_handle_t timer_handle;

	ESP_ERROR_CHECK(gptimer_new_timer(&builder->timer_config, &timer_handle));
	ESP_ERROR_CHECK(gptimer_set_alarm_action(timer_handle, &builder->alarm_config));
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(timer_handle, &builder->event_callbacks, NULL));

  (void)builder;
  timer_t timer = {
      .timer = timer_handle,
      .state = DISABLED,
      .enable = enable,
      .disable = disable,
      .start = start,
      .stop = stop,
  };

  snprintf(timer.name, sizeof(builder->name), "%s", builder->name);

  return timer;
}

static void start(timer_t *timer) {
  if(timer->state != ENABLED && timer->state != IDLE) {
    return;
  }

  ESP_ERROR_CHECK(gptimer_start(timer->timer));
  timer->state = RUNNING;
  log_message("Timer %s state: %s \n", timer->name, get_state_name(timer->state));
}

static void enable(timer_t *timer) {
  if(timer->state != DISABLED) {
    return;
  }

  ESP_ERROR_CHECK(gptimer_enable(timer->timer));
  timer->state = ENABLED;
  log_message("Timer %s state: %s \n", timer->name, get_state_name(timer->state));
}

static void stop(timer_t *timer) {
  if(timer->state != RUNNING) {
    return;
  }

  ESP_ERROR_CHECK(gptimer_stop(timer->timer));
  timer->state = IDLE;
  log_message("Timer %s state: %s \n", timer->name, get_state_name(timer->state));
}

static void disable(timer_t *timer) {
  if(timer->state != ENABLED && timer->state != IDLE) {
    return;
  }

  ESP_ERROR_CHECK(gptimer_disable(timer->timer));
  timer->state = DISABLED;
  log_message("Timer %s state: %s \n", timer->name, get_state_name(timer->state));
}

static const char *get_state_name(timer_state_t state) {
  return state_names[state];
}