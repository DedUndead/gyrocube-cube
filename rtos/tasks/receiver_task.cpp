#include "shared_resources.hpp"
#include "network_definitions.hpp"

#define MAX_NUMBER_OF_TOKENS 10

// Use Jasmine as multi-include
#define JSMN_HEADER
#include "jsmn.h"

void dispatch_event(jsmntok_t* tokens, const uint8_t& number_of_tokens)
{
    
}

/*
 * @brief Receiver task consumes raw arriving data
 * It decapsulates the frames and dispatches events based on the message type
 */
void v_receiver_task(void* pvParameters)
{   
    jsmn_parser json;
    jsmntok_t tokens[MAX_NUMBER_OF_TOKENS];

    jsmn_init(&parser);

    while (true) {
        message msg = mqtt_receive_queue->consume();

        uint8_t ret = jsmn_parse(&parser, msg.data, msg.data_len, tokens, MAX_NUMBER_OF_TOKENS);
        if (ret) {
            dispatch_event(tokens, MAX_NUMBER_OF_TOKENS);
        }
        else {
            // Put error in transmit queue, i.e., echo back error
        }
    }
}