#pragma once

#include <Arduino.h>
#include "Signals.h"

namespace com
{
    enum class FrameType
    {
        FRAME_COMMAND  = 0x00,
        FRAME_RESPONSE = 0x01,
    };

    typedef struct
    {
        Signal<FrameType, uint8_t*, size_t> OnFrameReceive;
    } LinkSignals_t;

    class SerialLink
    {
    public:
        SerialLink(Stream &serial);

        void SendFrame(FrameType type, const uint8_t *payload, const size_t length);
        void StartReceiving(uint8_t *buffer, const size_t size);

        LinkSignals_t& Signals();

        size_t HandleIncoming();
    private:
        FrameType GetType();
        uint8_t *GetPayload(size_t &length);

        typedef enum
        {
            STATE_IDLE,
            STATE_RECV
        } State_t;

        void SendEscaped(const uint8_t *buffer, const int length);
        void SendEscaped(const uint32_t word);
        void SendEscaped(const uint8_t byte);

        State_t HandleByte(uint8_t byte);
        void AppendBuffer(uint8_t byte);

        void Drop();
        bool CheckCRC32();

        State_t _state;
        Stream &_serial;

        uint8_t *_recv_buffer;
        size_t _recv_buffer_size;
        size_t _recv_idx;
        bool _recv_escaped;
        bool _recv_dropped;

        uint8_t _read_buffer[256];
        size_t _read_length = 0;
        size_t _read_idx = 0;

        LinkSignals_t _signals;
    };

} // namespace com