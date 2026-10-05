#include "adapter/logger.h"
#include <stdarg.h>
#include "esp_log.h"

#define TAG "APP"

void log_message(const char *format, ...) {
  va_list args;
  va_start(args, format);
  esp_log_writev(ESP_LOG_INFO, TAG, format, args);
  va_end(args);
}