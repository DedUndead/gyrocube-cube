#include "network_layer.hpp"

#define LL_MAX_FRAME_LENGTH 50

class LinkLayer {
public:
    LinkLayer(uint8_t* frame);
    uint8_t get_crc8();
    uint8_t get_payload_length();

    static LinkLayer build_frame(NetworkLayer packet);
    static LinkLayer build_frame(uint8_t* payload);
private:
    uint8_t buffer[LL_MAX_FRAME_LENGTH];
};

LinkLayer::LinkLayer(frame) : buffer(frame)
{ }

LinkLayer LinkLayer::build_frame(NetworkLayer packet)
{
    
}

LinkLayer LinkLayer::build_frame(uint8_t* playload)
{

}