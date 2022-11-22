#include "task_resources.hpp"
#include "json.hpp"

using json = nlohmann::json;

/*
 * @brief Receiver task consumes raw arriving data
 * It decapsulates the frames and dispatches events based on the message typ
 */
void v_receiver_task(void* pvParameters)
{   
    while (true) {
        message msg = accelerometer_side_queue->consume();

        auto json_msg = json::parse(msg.data, nullptr, false);
    }
}