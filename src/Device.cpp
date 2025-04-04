#include "Device.h"

namespace com
{
    Device::Device(com::SerialLink &seriallink)
        : _link{seriallink}
    {
        _link.Signals().OnFrameReceive.connect([this](FrameType type, uint8_t* payload, size_t length) {
            this->HandleFrame(type, payload, length);
        });
    }
    
    void Device::SetFwVersion(uint8_t major, uint8_t minor)
    {
        _version.major = major;
        _version.minor = minor;
    }

    void Device::SetDeviceType(DeviceType_t type)
    {
        _type = type;
    }

    void Device::HandleFrame(FrameType type, uint8_t *payload, const size_t length)
    {
        switch (type)
        {
        case FrameType::FRAME_COMMAND:
            HandleCmd(payload[0], payload + 1, length - 1);
            break;

        default:
            break;
        }
    }

    void Device::HandleCmd(uint8_t cmd, uint8_t *args, const size_t length)
    {
        switch (static_cast<DeviceCommand>(cmd))
        {
        case DeviceCommand::GET_FW_VERSION:
            ReplyFwVersion(_version.major, _version.minor);
            break;

        case DeviceCommand::GET_DEVICE_TYPE:
            ReplyDeviceType(_type);
            break;

        default:
            break;
        }
    }

    void Device::ReplyDeviceType(DeviceType_t type)
    {
        uint8_t payload[] = {static_cast<uint8_t>(com::DeviceCommand::GET_DEVICE_TYPE), type};
        _link.SendFrame(com::FrameType::FRAME_RESPONSE, payload, sizeof(payload));
    }

    void Device::ReplyFwVersion(uint8_t major, uint8_t minor)
    {
        uint8_t payload[] = {static_cast<uint8_t>(com::DeviceCommand::GET_FW_VERSION), major, minor};
        _link.SendFrame(com::FrameType::FRAME_RESPONSE, payload, sizeof(payload));
    }
}