#include "adapter/timer.h"
#include <stdint.h>
#include "driver/gptimer.h"

static timer_builder_t *with_config_in_milliseconds(timer_builder_t *builder, uint32_t milliseconds);
static timer_builder_t *with_config_in_seconds(timer_builder_t *builder, uint32_t seconds);
static timer_builder_t *with_alarm_on(timer_builder_t *builder, uint32_t alarm);
static timer_builder_t *with_action(timer_builder_t *builder, bool (*action)(struct gptimer_t *timer, const gptimer_alarm_event_data_t *event_data, void *arg));
static timer_t build(timer_builder_t *builder);
static void enable(timer_t *timer);
static void disable(timer_t *timer);
static void start(timer_t *timer);
static void stop(timer_t *timer);

timer_builder_t get_timer_builder(void) {
  timer_builder_t builder;
  builder.with_config_in_milliseconds = with_config_in_milliseconds;
  builder.with_config_in_seconds = with_config_in_seconds;
  builder.with_alarm_on = with_alarm_on;
  builder.with_action = with_action;
  builder.build = build;
  return builder;
}

static timer_builder_t *with_config_in_milliseconds(timer_builder_t *builder, uint32_t milliseconds) {
	gptimer_config_t timer_config;
	timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
	timer_config.direction = GPTIMER_COUNT_UP;
	timer_config.resolution_hz = milliseconds;

	builder->timer_config = timer_config;

  return builder;
}

static timer_builder_t *with_config_in_seconds(timer_builder_t *builder, uint32_t seconds) {
	gptimer_config_t timer_config;
	timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
	timer_config.direction = GPTIMER_COUNT_UP;
	timer_config.resolution_hz = seconds;

	builder->timer_config = timer_config;

  return builder;
}

static timer_builder_t *with_alarm_on(timer_builder_t *builder, uint32_t alarm) {
  	gptimer_alarm_config_t alarm_config = {
    .reload_count = 0,
    .alarm_count = alarm,
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

static timer_t build(timer_builder_t *builder) {
	gptimer_handle_t timer_handle;

	ESP_ERROR_CHECK(gptimer_new_timer(&builder->timer_config, &timer_handle));
	ESP_ERROR_CHECK(gptimer_set_alarm_action(timer_handle, &builder->alarm_config));
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(timer_handle, &builder->event_callbacks, NULL));

  (void)builder;
  timer_t timer = {
      .timer = timer_handle,
      .enable = enable,
      .disable = disable,
      .start = start,
      .stop = stop,
  };
  return timer;
}

static void start(timer_t *timer) {
  ESP_ERROR_CHECK(gptimer_start(timer->timer));
}

static void enable(timer_t *timer) {
  ESP_ERROR_CHECK(gptimer_enable(timer->timer));
}

static void stop(timer_t *timer) {
  ESP_ERROR_CHECK(gptimer_stop(timer->timer));
}

static void disable(timer_t *timer) {
  ESP_ERROR_CHECK(gptimer_disable(timer->timer));
}