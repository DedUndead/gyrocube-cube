idf_component_register(SRCS "ring_buffer.cpp" "flash.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES "nvs_flash")