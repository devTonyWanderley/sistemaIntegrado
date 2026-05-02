#pragma once
#include <windows.h>
#include <cstdint>
#include <atomic>
#include <vector>
#include <string>
#include "DeviceConfigs.h"

// Definimos o contrato: uma função que recebe um byte
typedef void (*SerialCallback)(uint8_t byte);

class SerialWin32
{
public:
    SerialWin32();
    ~SerialWin32();

    static std::vector<std::string> listarPortas();

    // Adicionamos o callback como parâmetro opcional ou via método
    bool abrir(const char* porta, const SerialParams& params, SerialCallback cb = nullptr);
    void fechar();

    bool enviar(const uint8_t* dados, uint32_t tamanho);

private:
    HANDLE m_hSerial;
    HANDLE m_threadH;
    SerialCallback m_callback;
    std::atomic<bool> m_ativo;

    static DWORD WINAPI threadLeitura(LPVOID param);
};
