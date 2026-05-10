//	configSerial.hpp    ..  versão atualizada de DeviceConfigs
#pragma once
#include <cstdint>

struct CfgSerial
{
    uint32_t raw;
    static constexpr uint32_t build(uint32_t baud, uint8_t par, uint8_t stop, uint8_t size)
    {
        return (baud & 0xFFFFF) | ((par & 0x0F)  << 20) | ((stop & 0x0F) << 24) | ((size & 0x0F) << 28);
    }

    uint32_t winBaud() const
    {
        return raw & 0xFFFFF;
    }

    uint8_t winParity() const
    {
        return (raw >> 20) & 0x0F;
    }

    uint8_t winStop() const
    {
        return (raw >> 24) & 0x0F;
    }

    uint8_t winSize() const
    {
        return (raw >> 28) & 0x0F;
    }
};