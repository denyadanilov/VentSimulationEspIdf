idf_component_register(
    SRCS "logger.h" "task.h" "utils.h" "pin.h" "timer.h"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gptimer freertos
)