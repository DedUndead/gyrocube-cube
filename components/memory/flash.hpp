#ifndef FLASH_HPP
#define FLASH_HPP


#include "nvs.h"
#include "esp_err.h"


#define PARITION_NAME "GYROSTORAGE"


class Flash {
public:
    Flash();
    ~Flash();
    esp_err_t read(const char* key, void* out_value, size_t* length);
    esp_err_t write(const char* key, void* out_value, size_t length);
private:
    nvs_handle_t handle;
};


#endif