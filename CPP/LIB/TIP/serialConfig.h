//  serialConfig.h
#pragma once
#include <cstdint>

namespace SerialConfig
{
enum class Parity {Nenhuma = 0, Impar = 1, Par = 2};
enum class StopBit {Um = 1, Dois = 2};
struct Parametros
{
    int32_t baudrate = 9600;
    int32_t dataBits = 8;
    Parity paridade = Parity::Nenhuma;
    StopBit stopbit = StopBit::Um;
};
}