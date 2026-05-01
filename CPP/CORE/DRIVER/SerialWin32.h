//  SerialWin32.h
#pragma once
#include <windows.h>
#include <cstdint>
#include <atomic>
#include "DeviceConfigs.h"

class SerialWin32
{
public:
    SerialWin32();
    ~SerialWin32();

    bool abrir(const char* porta, const SerialParams& params); // Ex: "COM3"
    void fechar();

    // Envia bytes brutos (mínima camada)
    bool enviar(const uint8_t* dados, uint32_t tamanho);

private:
    HANDLE m_hSerial;           // O "link" com o Windows
    HANDLE m_threadH;           // O identificador da Thread no Windows
    HANDLE m_hArquivoRaw;       // Arquivo .raw aberto via CreateFile
    std::atomic<bool> m_ativo;  // Controle da Thread

    // Aqui entrará a lógica da Thread de leitura
    static DWORD WINAPI threadLeitura(LPVOID param);
};
