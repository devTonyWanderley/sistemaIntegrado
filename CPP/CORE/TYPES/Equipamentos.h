//  Equipamentos.h
#pragma once
#include <cstdint>
#include "DeviceConfigs.h"

struct PerfilEquipamento {
    const char* nome;
    uint32_t params; // Já calculado via SerialParams::build
    const char* extensao;
};

static const PerfilEquipamento LISTA_EQUIPAMENTOS[] = {
    {"Leica GSI",     SerialParams::build(19200, 0, 0, 8), ".gsi"},
    {"Topcon GTS",    SerialParams::build(9600,  0, 0, 8), ".raw"},
    {"South / Kolida", SerialParams::build(9600,  0, 0, 8), ".txt"},
    {"NMEA (GPS)",    SerialParams::build(4800,  0, 0, 8), ".nmea"}
};
