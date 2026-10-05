#ifndef SRC_APP_APP_H
#define SRC_APP_APP_H

#define APP_TASK_STACK_DEPTH 2048
#define APP_TASK_PRIORITY 1
#define IDLE_PERIOD_SECONDS 15
#define DUTY_TIME_SECONDS 5

void app_init(void);
void app_run(void);

#endif // SRC_APP_APP_H