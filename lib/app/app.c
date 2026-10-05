#include "app.h"
#include "fan.h"
#include "adapter/task.h"
#include "adapter/logger.h"
#include "adapter/timer.h"
#include "adapter/utils.h"


void update_task(void *parameters);
bool IRAM_LOCATED idle_timer_callback(struct gptimer_t *timer,
               const gptimer_alarm_event_data_t *event_data, void *arg);
bool IRAM_LOCATED duty_timer_callback(struct gptimer_t *timer,
               const gptimer_alarm_event_data_t *event_data, void *arg);

fan_t fan;
task_context_t update_task_context = {update_task, NULL};
timer_t idle_timer;
timer_t duty_timer;
bool volatile idle_timer_completed = false;
bool volatile duty_timer_completed = false;

void app_init(void) {
    fan = create_fan();
    fan.setup(&fan);
    timer_builder_t idle_timer_builder = get_timer_builder();
    idle_timer = idle_timer_builder
                    .with_name(&idle_timer_builder, "idle")
                    ->with_config_in_seconds(&idle_timer_builder)
                    ->with_alarm_on(&idle_timer_builder, IDLE_PERIOD_SECONDS)
                    ->with_action(&idle_timer_builder, idle_timer_callback)
                    ->build(&idle_timer_builder);
    timer_builder_t duty_timer_builder = get_timer_builder();
    duty_timer = duty_timer_builder
                    .with_name(&duty_timer_builder, "duty")
                    ->with_config_in_seconds(&duty_timer_builder)
                    ->with_alarm_on(&duty_timer_builder, DUTY_TIME_SECONDS)
                    ->with_action(&duty_timer_builder, duty_timer_callback)
                    ->build(&duty_timer_builder);

    idle_timer.enable(&idle_timer);
    duty_timer.enable(&duty_timer);
}

void app_run(void) {
    idle_timer.start(&idle_timer);

    result_t result = schedule_task(&update_task_context, "update_task", APP_TASK_STACK_DEPTH, APP_TASK_PRIORITY);
    if (result == FAILURE) {
        log_message("Failed to create update task\n");
    }
}

void update_task(void *parameters) {
    if (idle_timer_completed) {
        idle_timer.stop(&idle_timer);
        duty_timer.start(&duty_timer);
        fan.turn_on(&fan);
        idle_timer_completed = false;
    }
    if (duty_timer_completed) {
        idle_timer.start(&idle_timer);
        duty_timer.stop(&duty_timer);
        fan.turn_off(&fan);
        duty_timer_completed = false;
    }
}

bool IRAM_LOCATED idle_timer_callback(struct gptimer_t *timer,
               const gptimer_alarm_event_data_t *event_data, void *arg) {
    idle_timer_completed = true;
    return true;
}

bool IRAM_LOCATED duty_timer_callback(struct gptimer_t *timer,
               const gptimer_alarm_event_data_t *event_data, void *arg) {
    duty_timer_completed = true;
    return true;
}
