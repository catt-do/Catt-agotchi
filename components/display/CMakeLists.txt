idf_component_register(
    SRCS "display.c" "sprite.c"
    INCLUDE_DIRS "."
    REQUIRES
        st7789
        catt
        esp_driver_gpio
    PRIV_REQUIRES
        esp_timer
)
