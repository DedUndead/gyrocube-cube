#include "receiver_task.hpp"
#include "shared_resources.hpp"
#include "network_definitions.hpp"
#include "jsmn.h"

#include "vibration_motor.hpp"

#include <string>
#include <stdio.h>

static std::string get_token_string(const jsmntok_t& token, char* json_)
{
    std::string json(json_);
    size_t length = token.end - token.start;

    return json.substr(token.start, length);
}

/* Event dispatcher function */
static bool parse_json(char* json_, jsmntok_t* tokens, const uint8_t& number_of_tokens)
{
    uint8_t index = 0;
    std::string key;

    // Verify that first token is tag
    key = get_token_string(tokens[index], json_);
    if (key != "mtag") {
        return false;
    }
    index++;

    // Parse tag value
    key = get_token_string(tokens[index], json_);
    uint8_t tag;
    if (sscanf(key.c_str(), "%s", &tag) != 1) {
        return false;
    }

    switch (tag) {
        case message_tag::CUBE_UPDATE_CONFIG_REQ:
            //return issue_cube_update(json_, tokens, number_of_tokens);
            return true;

            break;
        case message_tag::PING:
            motor_vibrate(MOTOR_DEFAULT_DURATION_MS);
            return true;

            break;
        default:
            return false;
    }
}

/*
 * @brief Receiver task consumes raw arriving data
 * It decapsulates the frames and dispatches events based on the message type
 */
void v_receiver_task(void* pvParameters)
{   
    motor_initialize(MOTOR_PIN);

    jsmn_parser parser;
    jsmntok_t tokens[MAX_NUMBER_OF_TOKENS];
    jsmn_init(&parser);

    message msg;

    while (true) {
        mqtt_receive_queue->consume(&msg);

        uint8_t ret = jsmn_parse(&parser, msg.data, msg.data_len, tokens, MAX_NUMBER_OF_TOKENS);
        bool event_dispatched = false;

        if (ret >= MIN_NUMBER_OF_TOKENS) {
            event_dispatched = parse_json(msg.data, tokens, ret);
        }
        else {
            // Put GARBISH error
        }

        if (!event_dispatched) {
            // Put UNKNOWN JSON error
        }
    }
}