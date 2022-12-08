#ifndef MEMORY_LAYOUT_HELPERS_HPP
#define MEMORY_LAYOUT_HELPERS_HPP


#include "flash.hpp"

typedef struct {
    uint32_t color;
} cube_config;

void init_flash_with_default_configuration(Flash* flash);
cube_config load_configuration(Flash* flash, const uint8_t& side);


#endif
