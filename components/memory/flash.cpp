#include "freertos/task.h"
#include "esp_flash.h"


Flash::Flash()
{
    nvs_flash_init();
    nvs_open(PARITION_NAME, NVS_READWRITE, &handle);
}

Flash::~Flash()
{
    nvs_close(handle);
}

/*
 * @brief Read data buffer from flash
 * @param key Data key
 * @param out_value Buffer to save data to
 * @param length Expected data length
 * @return Status code of operataion
 */
esp_err_t Flash::read(const char* key, void* out_value, size_t* length)
{
    return nvs_get_blob(handle, key, out_value, length);
}

/*
 * @brief Write data buffer to flash
 * @param key Data key
 * @param out_value Buffer to write data from
 * @param length Data length
 * @return Status code of operataion
 */
esp_err_t Flash::write(const char* key, void* out_value, size_t* length)
{
    esp_err_t result = nvs_set_blob(handle, key, out_value, length);
    esp_err_t result &= nvs_commit(handle);

    return result;
}
