idf_component_register(
    SRCS "esp_timer.c" "esp_pin.c" "esp_task.c" "esp_logger.c" "esp_utils.c"
    INCLUDE_DIRS ".."
    PRIV_INCLUDE_DIRS "../adapter"
    REQUIRES esp_driver_gpio esp_driver_gptimer esp_timer freertos log
)