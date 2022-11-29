#include "shared_resources.hpp"
#include "network_definitions.hpp"

#include "vibration_motor.hpp"

#include <string>
#include <stdio.h>


/* Event dispatcher function */
static bool parse_json(char* json_, jsmntok_t* tokens, const uint8_t& number_of_tokens)
{
    uint8_t index = 0;
    std::string key;

    // Verify that first token is tag
    key = get_token_string(tokens, raw);
    if (key != "mtag") return false;
    index++;

    // Parse tag value
    key = get_token_string(tokens, raw);
    uint8_t tag;
    if (sscanf(key.c_str(), "%d", &tag) != 1) return false;

    switch (tag) {
        case message_tag.CUBE_UPDATE_CONFIG_REQ:
            return issue_cube_update(json_, tokens, number_of_tokens);
            break;
        case message_tag.PING:
            motor_vibrate(MOTOR_DEFAULT_DURATION_MS);
            break;
        default:
            return false;
    }
}

static std::string get_token_string(const jsmntok_t& tok, char* raw)
{
    std::string json(json_);
    size_t length = tokens[index].end - tokens[index].start;

    return json.substr(key.start, length);
}

/*
 * @brief Receiver task consumes raw arriving data
 * It decapsulates the frames and dispatches events based on the message type
 */
void v_receiver_task(void* pvParameters)
{   
    motor_initialize(MOTOR_PIN);

    jsmn_parser json;
    jsmntok_t tokens[MAX_NUMBER_OF_TOKENS];

    jsmn_init(&parser);

    while (true) {
        message msg = mqtt_receive_queue->consume();

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