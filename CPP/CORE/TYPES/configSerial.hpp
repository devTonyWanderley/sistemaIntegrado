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

struct PerfilEquipamento
{
    const char* nome;
    uint32_t params; // Já calculado via SerialParams::build
};

static const PerfilEquipamento LISTA_EQUIPAMENTOS[] =
{
    // --- ESTAÇÕES TOTAIS (Total Stations) ---
    {"Default",          CfgSerial::build(9600,  0, 0, 8)}, // 9600 8N1
    {"Leica GSI/TPS",    CfgSerial::build(19200, 0, 0, 8)}, // 19200 8N1 (Linhas FlexLine, TS, MS)
    {"Leica (Legado)",   CfgSerial::build(2400,  2, 0, 7)}, // 2400 7E1 (Modelos antigos como TC600/800)
    {"Topcon GTS/GPT",   CfgSerial::build(9600,  0, 0, 8)}, // 9600 8N1
    {"Topcon (Legado)",  CfgSerial::build(1200,  2, 0, 7)}, // 1200 7E1
    {"Trimble S/VX",     CfgSerial::build(9600,  0, 0, 8)}, // 9600 8N1
    {"Nikon Nivo/DTM",   CfgSerial::build(4800,  0, 0, 8)}, // 4800 8N1
    {"Sokkia Set/CX",    CfgSerial::build(9600,  0, 0, 8)}, // 9600 8N1
    {"South / Kolida",   CfgSerial::build(9600,  0, 0, 8)}, // 9600 8N1
    {"Pentax",           CfgSerial::build(1200,  0, 0, 8)}, // 1200 8N1

    // --- RECEPTORES GNSS / GPS ---
    {"NMEA Standard",    CfgSerial::build(4800,  0, 0, 8)}, // 4800 8N1 (Padrão NMEA-0183)
    {"NMEA High-Speed",  CfgSerial::build(38400, 0, 0, 8)}, // 38400 8N1 (Comum em GPS RTK moderno)
    {"Trimble TSIP",     CfgSerial::build(9600,  1, 0, 8)}, // 9600 8O1 (Protocolo binário Trimble)
    {"Garmin / SiRF",    CfgSerial::build(9600,  0, 0, 8)}, // 9600 8N1

    // --- SCANNERS & LIDAR ---
    {"Faro Focus",       CfgSerial::build(115200, 0, 0, 8)}, // Geralmente via TCP/IP, mas serial usa alta velocidade
    {"Velodyne Lidar",   CfgSerial::build(115200, 0, 0, 8)}, // Comunicação serial para telemetria/NMEA
    {"Riegl",            CfgSerial::build(57600,  0, 0, 8)},
    {"Hokuyo URG",       CfgSerial::build(19200,  0, 0, 8)}  // Scanners menores para robótica/automação
};
