idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES spi_flash esp_timer
                       INCLUDE_DIRS "."
                       REQUIRES buttons catt display storage)
