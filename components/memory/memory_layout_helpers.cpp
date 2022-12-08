#include "memory_layout_helpers.hpp"

/*
 * @brief Initialize flash memory with default configuration for each side
 */
void init_flash_with_default_configuration(Flash* flash)
{
    const uint8_t number_of_sides = 6;
    const uint32_t default_colors[number_of_sides] = { 0x0000cc, 0x00cc00, 0xcc0000,
                                                       0xcc00cc, 0x00cccc, 0xcccc00 };

    for (int i = 1; i <= number_of_sides; i++) {
        cube_config config = { .color = default_colors[i - 1] };
        char key = i + '0';

        flash->write(&key, &config, sizeof(cube_config));
    }
}

/*
 * @brief Load cube configuration from side key.
 * Keys: [c 1 - 6]
 */
cube_config load_configuration(Flash* flash, const uint8_t& side)
{
    cube_config config;
    size_t length_read;
    char key = side + '0';

    flash->read(&key, &config, &length_read);

    return config;
}
