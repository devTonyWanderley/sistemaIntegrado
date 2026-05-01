//  SerialWin32.cpp
#include "SerialWin32.h"
#include <iostream>

SerialWin32::SerialWin32(): m_hSerial(INVALID_HANDLE_VALUE), m_threadH(NULL), m_callback(nullptr), m_ativo(false){}

SerialWin32::~SerialWin32(){fechar();}

bool SerialWin32::abrir(const char* porta, const SerialParams& params, SerialCallback cb)
{
    m_hSerial = CreateFileA(porta, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_hSerial == INVALID_HANDLE_VALUE) return false;
    m_callback = cb;
    DCB dcb{};
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(m_hSerial, &dcb)) return false;
    dcb.BaudRate = params.winBaud();   // Extrai os 16 bits iniciais
    dcb.Parity   = params.winParity(); // Extrai os bits de paridade
    dcb.StopBits = params.winStop();   // Extrai os bits de parada
    dcb.ByteSize = params.winSize();   // Extrai o tamanho do byte
    dcb.fOutxCtsFlow = FALSE;
    dcb.fRtsControl  = RTS_CONTROL_ENABLE;
    if (!SetCommState(m_hSerial, &dcb)) return false;
    COMMTIMEOUTS timeouts{};
    timeouts.ReadIntervalTimeout = MAXDWORD; // Retorna imediatamente o que tiver
    SetCommTimeouts(m_hSerial, &timeouts);
    m_ativo = true;
    m_threadH = CreateThread(NULL, 0, threadLeitura, this, 0, NULL);
    if (m_threadH == NULL)
    {
        fechar();
        return false;
    }
    return true;
}

void SerialWin32::fechar()
{
    m_ativo = false;
    if (m_threadH != NULL)
    {
        WaitForSingleObject(m_threadH, 2000);
        CloseHandle(m_threadH);
        m_threadH = NULL;
    }
    if (m_hSerial != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hSerial);
        m_hSerial = INVALID_HANDLE_VALUE;
    }
    m_callback = nullptr;
}

DWORD WINAPI SerialWin32::threadLeitura(LPVOID param)
{
    SerialWin32* instancia = static_cast<SerialWin32*>(param);
    uint8_t byteLido;
    DWORD bytesRecebidos;
    while (instancia->m_ativo)
    {
        if (ReadFile(instancia->m_hSerial, &byteLido, 1, &bytesRecebidos, NULL))
            if (bytesRecebidos > 0 && instancia->m_callback) instancia->m_callback(byteLido);
        Sleep(1);
    }
    return 0;
}
