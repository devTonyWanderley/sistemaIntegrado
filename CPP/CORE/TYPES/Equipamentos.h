//  Equipamentos.h
#pragma once
#include <cstdint>
#include "DeviceConfigs.h"

struct PerfilEquipamento {
    const char* nome;
    uint32_t params; // Já calculado via SerialParams::build
};

static const PerfilEquipamento LISTA_EQUIPAMENTOS[] = {
    // --- ESTAÇÕES TOTAIS (Total Stations) ---
    {"Default",          SerialParams::build(9600,  0, 0, 8)}, // 9600 8N1
    {"Leica GSI/TPS",    SerialParams::build(19200, 0, 0, 8)}, // 19200 8N1 (Linhas FlexLine, TS, MS)
    {"Leica (Legado)",   SerialParams::build(2400,  2, 0, 7)}, // 2400 7E1 (Modelos antigos como TC600/800)
    {"Topcon GTS/GPT",   SerialParams::build(9600,  0, 0, 8)}, // 9600 8N1
    {"Topcon (Legado)",  SerialParams::build(1200,  2, 0, 7)}, // 1200 7E1
    {"Trimble S/VX",     SerialParams::build(9600,  0, 0, 8)}, // 9600 8N1
    {"Nikon Nivo/DTM",   SerialParams::build(4800,  0, 0, 8)}, // 4800 8N1
    {"Sokkia Set/CX",    SerialParams::build(9600,  0, 0, 8)}, // 9600 8N1
    {"South / Kolida",   SerialParams::build(9600,  0, 0, 8)}, // 9600 8N1
    {"Pentax",           SerialParams::build(1200,  0, 0, 8)}, // 1200 8N1

    // --- RECEPTORES GNSS / GPS ---
    {"NMEA Standard",    SerialParams::build(4800,  0, 0, 8)}, // 4800 8N1 (Padrão NMEA-0183)
    {"NMEA High-Speed",  SerialParams::build(38400, 0, 0, 8)}, // 38400 8N1 (Comum em GPS RTK moderno)
    {"Trimble TSIP",     SerialParams::build(9600,  1, 0, 8)}, // 9600 8O1 (Protocolo binário Trimble)
    {"Garmin / SiRF",    SerialParams::build(9600,  0, 0, 8)}, // 9600 8N1

    // --- SCANNERS & LIDAR ---
    {"Faro Focus",       SerialParams::build(115200, 0, 0, 8)}, // Geralmente via TCP/IP, mas serial usa alta velocidade
    {"Velodyne Lidar",   SerialParams::build(115200, 0, 0, 8)}, // Comunicação serial para telemetria/NMEA
    {"Riegl",            SerialParams::build(57600,  0, 0, 8)},
    {"Hokuyo URG",       SerialParams::build(19200,  0, 0, 8)}  // Scanners menores para robótica/automação
};
