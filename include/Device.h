#pragma once

#include "Link.h"
#include "Signals.h"

namespace com
{
    enum class DeviceCommand
    {
        GET_FW_VERSION  = 0x00,
        GET_DEVICE_TYPE = 0x01,
    };

    typedef uint8_t DeviceType_t;

    class Device
    {
    public:
        Device(SerialLink &seriallink);

        void SetFwVersion(uint8_t major, uint8_t minor);
        void SetDeviceType(DeviceType_t type);

    protected:
        virtual void HandleCmd(uint8_t cmd, uint8_t *args, const size_t length);

        SerialLink& _link;

    private:
        void HandleFrame(FrameType type, uint8_t *payload, const size_t length);
        void ReplyFwVersion(uint8_t major, uint8_t minor);
        void ReplyDeviceType(DeviceType_t type);

        DeviceType_t _type = 0x00;

        struct{
            uint8_t major = 0;
            uint8_t minor = 0;
        } _version;
    };
}