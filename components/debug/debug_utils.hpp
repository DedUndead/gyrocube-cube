#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H


/* Simple debug utils for developing */

#include "driver/rmt.h"
#include "esp_log.h"

void log_rmt_item(const rmt_item32_t& item)
{
    ESP_LOGI(
        "RMT_ITEM", 
        "Item value: %u | "
        "Signal 0 - level: %u - duration: %u | "
        "Signal 1 - level: %u - duration: %u |",
        item.val,
        item.level0, item.duration0,
        item.level1, item.duration1
    );
}

void log_rmt_items(rmt_item32_t* items, const uint16_t& size)
{
    for (uint16_t i = 0; i < size; i++) {
        log_rmt_item(items[i]);
    }
}


#endif
