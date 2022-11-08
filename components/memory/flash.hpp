#ifndef FLASH_HPP
#define FLASH_HPP


#include "nvs.h"
#include "esp_err.h"


#define PARITION_NAME "GYROSTORAGE"


class Flash {
public:
    Flash();
    ~Flash();
    esp_err_t read();
    esp_err_t write();
private:
    nvs_handle_t handle;
}


#endif