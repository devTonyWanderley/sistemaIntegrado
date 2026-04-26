//	tipConfig.h
#pragma once
#include <string>
#include "MotorComunicacao.h" // Para usar a struct ConfigSerial

struct PerfilEquipamento
{
    std::string nome;      // "Leica TC407", "Geodetic GD5"
    ConfigSerial config;   // Baud, Paridade, etc.
};
