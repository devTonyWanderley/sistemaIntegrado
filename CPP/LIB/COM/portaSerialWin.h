//  portaSerialWin.h
#pragma once
#include "motorComunicacao.h"
#include <windows.h>
#include <string>

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