idf_component_register(
    SRCS "display.c"
    INCLUDE_DIRS "."
    REQUIRES
        st7789
        catt
        esp_driver_gpio
)
