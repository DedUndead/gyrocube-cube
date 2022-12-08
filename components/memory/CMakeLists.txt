idf_component_register(SRCS "shared_buffer.cpp" "memory_layout_helpers.cpp" "flash.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES "nvs_flash")