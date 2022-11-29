#ifndef NETWORK_DEFINITIONS_HPP
#define NETWORK_DEFINITIONS_HPP


/* Network Definitions */
#define MQTT_CONNECTED_NETWORK_EVENT_BIT 0x01

enum message_tag {
    CUBE_JOINED            = 0x01,
    CUBE_DISCONNECTED      = 0x02,
    SENSOR_DATA            = 0x03,
    SENSOR_DATA_PERIOD_REQ = 0x04,
    SENSOR_DATA_PERIOD_RES = 0x05,
    CUBE_FLIPPED           = 0x06,
    CUBE_SIDE_CONFIG       = 0x07,
    CUBE_UPDATE_CONFIG_REQ = 0x08,
    CUBE_UPDATE_CONFIG_RES = 0x09,
    PING                   = 0xff
};

enum message_type {
    REQUEST  = 0x00,  // Request action
    RESPONSE = 0x01,  // Response to request
    SIGNAL   = 0x02   // Message that requires no response, can be used as ACK
};

#define MQTT_TOPIC_CONFIG       "gyro/config"
#define MQTT_TOPIC_MEASUREMENTS "gyro/measurement"
#define MQTT_TOPIC_CUBE_STATE   "gyro/cube"
#define MQTT_TOPIC_NETWORK      "gyro/network"
#define MQTT_TOPIC_ERROR        "gyro/error"

typedef struct {
    char* data;
    char* topic;
    int data_len;
} message;


#endif
