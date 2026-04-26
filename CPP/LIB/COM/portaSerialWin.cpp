//	portaSerialWin.cpp
#include "portaSerialWin.h"
#include <cstdio>

PortaSerialWin::PortaSerialWin() : mHandlePorta(INVALID_HANDLE_VALUE)
{}

PortaSerialWin::~PortaSerialWin()
{
    desconectar();
}

int32_t PortaSerialWin::lerParaBuffer(char *bufferDestino, uint32_t tamanhoMaximo)
{
    if(!estaConectado()) return -1;
    DWORD bytesLidos = 0;
    if(ReadFile(mHandlePorta, bufferDestino, tamanhoMaximo, &bytesLidos, NULL)) return static_cast<int32_t>(bytesLidos);
    return -1;
}

bool PortaSerialWin::conectar(int portaNumero, const ConfigSerial& config)
{
    char nomePorta[20];
    sprintf(nomePorta, "\\\\.\\COM%d", portaNumero);
    mHandlePorta = CreateFileA(nomePorta, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(mHandlePorta == INVALID_HANDLE_VALUE) return false;
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if(!GetCommState(mHandlePorta, &dcbSerialParams)) return false;
    dcbSerialParams.BaudRate = config.baudRate;
    dcbSerialParams.ByteSize = config.byteSize;
    switch(config.stopBits)
    {
    case StopBits::UmMeio: dcbSerialParams.StopBits = ONE5STOPBITS; break;
    case StopBits::Dois: dcbSerialParams.StopBits = TWOSTOPBITS; break;
    default: dcbSerialParams.StopBits = ONESTOPBIT;
    }
    switch(config.paridade)
    {
    case Paridade::Impar: dcbSerialParams.Parity = ODDPARITY; break;
    case Paridade::Par: dcbSerialParams.Parity = EVENPARITY; break;
    default: dcbSerialParams.Parity = NOPARITY;
    }
    if(!SetCommState(mHandlePorta, &dcbSerialParams)) return false;
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(mHandlePorta, &timeouts);
    return true;
}

bool PortaSerialWin::enviarDoBuffer(const char *bufferOrigem, uint32_t tamanho)
{
    if(!estaConectado()) return false;
    DWORD bytesEscritos = 0;
    return WriteFile(mHandlePorta, bufferOrigem, tamanho, &bytesEscritos, NULL);
}

void PortaSerialWin::desconectar()
{
    if(estaConectado())
    {
        CloseHandle(mHandlePorta);
        mHandlePorta = INVALID_HANDLE_VALUE;
    }
}

bool PortaSerialWin::estaConectado() const
{
    return mHandlePorta != INVALID_HANDLE_VALUE;
}