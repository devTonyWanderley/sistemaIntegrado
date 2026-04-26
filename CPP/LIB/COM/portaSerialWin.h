//  portaSerialWin.h
#pragma once

// 1. Headers da biblioteca padrão C++ PRIMEIRO
#include <string>
#include <cstdint>

// 2. Definições para "limpar" o windows.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX // Impede que o Windows defina macros min/max que quebram o C++
#endif

// 3. Header do Windows
#include <windows.h>

// 4. Seus headers
#include "motorComunicacao.h"

class PortaSerialWin : public MotorComunicacao
{
public:
    PortaSerialWin();
    ~PortaSerialWin() override;
    int32_t lerParaBuffer(char* bufferDestino, uint32_t tamanhoMaximo) override;
    bool enviarDoBuffer(const char* bufferOrigem, uint32_t tamanho) override;
    bool conectar(int portaNumero, const ConfigSerial& config) override;
    void desconectar() override;
    bool estaConectado() const override;
private:
    HANDLE mHandlePorta; // O "ponteiro" do Windows para o hardware
};