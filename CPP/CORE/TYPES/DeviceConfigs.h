//  DeviceConfigs.h
#pragma once
#include <cstdint>

struct SerialParams
{
    uint32_t raw;

    // Novo Mapa: [4b: Size][4b: Stop][4b: Parity][20b: Baud]
    static constexpr uint32_t build(uint32_t baud, uint8_t par, uint8_t stop, uint8_t size) {
        return (baud & 0xFFFFF)           | // Bits 0-19 (Baud)
               ((par & 0x0F)  << 20)      | // Bits 20-23 (Parity)
               ((stop & 0x0F) << 24)      | // Bits 24-27 (Stop)
               ((size & 0x0F) << 28);       // Bits 28-31 (Size)
    }

    // Getters atualizados para as novas posições
    uint32_t winBaud()   const { return raw & 0xFFFFF; }         // Máscara de 20 bits
    uint8_t  winParity() const { return (raw >> 20) & 0x0F; }
    uint8_t  winStop()   const { return (raw >> 24) & 0x0F; }
    uint8_t  winSize()   const { return (raw >> 28) & 0x0F; }
};