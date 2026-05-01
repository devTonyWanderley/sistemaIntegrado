//  DeviceConfigs.h
#pragma once
#include <cstdint>

// Estrutura compacta: [8 bits: ByteSize][4 bits: Stop][4 bits: Parity][16 bits: Baud]
// Total: 32 bits (4 bytes)
struct SerialParams
{
    uint32_t raw;

    // Construtor utilitário para facilitar a criação manual
    static constexpr uint32_t build(uint32_t baud, uint8_t par, uint8_t stop, uint8_t size)
    {return (baud & 0xFFFF) | ((par & 0x0F) << 16) | ((stop & 0x0F) << 20) | ((size & 0xFF) << 24);}

    // Extração imediata para a WinAPI (Zero processamento lógico)
    uint32_t winBaud()   const { return raw & 0xFFFF; }           // Bits 0-15
    uint8_t  winParity() const { return (raw >> 16) & 0x0F; }    // Bits 16-19
    uint8_t  winStop()   const { return (raw >> 20) & 0x0F; }    // Bits 20-23
    uint8_t  winSize()   const { return (raw >> 24) & 0xFF; }    // Bits 24-31
};
