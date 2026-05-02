//  SerialWin32.cpp
#include "SerialWin32.h"
#include <iostream>

SerialWin32::SerialWin32(): m_hSerial(INVALID_HANDLE_VALUE), m_threadH(NULL), m_callback(nullptr), m_ativo(false){}

SerialWin32::~SerialWin32(){fechar();}

std::vector<std::string> SerialWin32::listarPortas() {
    std::vector<std::string> portas;
    char nomePorta[16];
    char caminhoDispositivo[MAX_PATH];

    // Varre de COM1 até COM16 (o comum em notebooks/estações)
    for (int i = 1; i <= 16; ++i) {
        sprintf(nomePorta, "COM%d", i);

        // QueryDosDevice retorna 0 se a porta não existir no registro do Windows
        if (QueryDosDeviceA(nomePorta, caminhoDispositivo, MAX_PATH) != 0) {
            portas.push_back(nomePorta);
        }
    }
    return portas;
}

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
    //................................................................................
    SetupComm(m_hSerial, 4096, 4096);
    PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
    //...................................................................................
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

DWORD WINAPI SerialWin32::threadLeitura(LPVOID param) {
    SerialWin32* instancia = static_cast<SerialWin32*>(param);

    uint8_t buffer[256]; // Buffer local maior
    DWORD bytesLidos;

    while (instancia->m_ativo) {
        // Pedimos até 256 bytes de uma vez
        if (ReadFile(instancia->m_hSerial, buffer, sizeof(buffer), &bytesLidos, NULL)) {
            if (bytesLidos > 0 && instancia->m_callback) {
                // Entrega o bloco para o callback
                for (DWORD i = 0; i < bytesLidos; ++i) {
                    instancia->m_callback(buffer[i]);
                }
            } else {
                // Só descansa se não houver NADA no buffer
                Sleep(1);
            }
        }
    }
    return 0;
}

