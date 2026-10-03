idf_component_register(
    SRCS "storage.c"
    INCLUDE_DIRS "."
    REQUIRES catt
    PRIV_REQUIRES nvs_flash
)
