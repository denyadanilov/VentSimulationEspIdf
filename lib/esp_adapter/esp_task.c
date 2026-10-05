#include "adapter/task.h"

void task_wrapper(void *parameter) {
  task_context_t *context = (task_context_t *)parameter;
  if (context == NULL || context->task_function == NULL) {
    vTaskDelete(NULL);
    return;
  }

  void (*function)(void *) = context->task_function;
  void *function_parameters = context->parameters;

  while (true) {
    function(function_parameters);
    vTaskDelay(1);
  }
}

result_t schedule_task(task_context_t *context, const char *task_name,
                     uint32_t stack_depth, uint8_t priority) {

  BaseType_t task_result =
      xTaskCreate(task_wrapper, task_name, stack_depth,
                  (void *)context, priority, NULL);

  return task_result == pdPASS ? SUCCESS : FAILURE;
}