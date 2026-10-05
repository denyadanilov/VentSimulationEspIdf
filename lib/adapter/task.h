#ifndef SRC_ADAPTER_TASK_H
#define SRC_ADAPTER_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum result { SUCCESS, FAILURE };

struct task_context {
  void (*task_function)(void *);
  void *parameters;
};

typedef enum result result_t;
typedef struct task_context task_context_t;

result_t schedule_task(task_context_t *context, const char *task_name,
                       uint32_t stack_depth, uint8_t priority);

#endif // SRC_ADAPTER_TASK_H