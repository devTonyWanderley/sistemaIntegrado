//  SerialModerno.cpp
#include "SerialModerno.h"

bool SerialModerno::Abrir(const std::string &porta, const SerialParams &params, DataCallback cb)
{
    mHSerial = CreateFileA(
        porta.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL
        );
    if(mHSerial == INVALID_HANDLE_VALUE) return false;
    mOnData = cb;
    DCB dcb = {};
    dcb.DCBlength = sizeof(DCB);
    if(!GetCommState(mHSerial, &dcb)) return false;
    dcb.BaudRate = params.winBaud();
    dcb.Parity = params.winParity();
    dcb.StopBits = params.winStop();
    dcb.ByteSize = params.winSize();
    if(!SetCommState(mHSerial, &dcb)) return false;
    COMMTIMEOUTS timeouts = {};
    timeouts.ReadIntervalTimeout = MAXDWORD; // Retorna imediatamente o que estiver no buffer
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    SetCommTimeouts(mHSerial, &timeouts);
    mRodando = true;
    mWorker = std::thread(&SerialModerno::loopLeitura, this);
    return true;
}

bool SerialModerno::Enviar(std::span<const uint8_t> dados)
{
    if(mHSerial == INVALID_HANDLE_VALUE) return false;
    DWORD bytesEscritos = 0;
    if(WriteFile(mHSerial, dados.data(), static_cast<DWORD>(dados.size()), &bytesEscritos, NULL))
        return bytesEscritos == dados.size();
    return false;
}

void SerialModerno::loopLeitura()
{
    uint8_t buffer[512];
    DWORD bytesLidos = 0;
    while(mRodando){
        if(ReadFile(mHSerial, buffer, sizeof(buffer), &bytesLidos, NULL))
            if(bytesLidos > 0 && mOnData) mOnData(std::span<const uint8_t>(buffer, bytesLidos));
        if(bytesLidos == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void SerialModerno::Fechar()
{
    mRodando = false;
    if(mWorker.joinable()) mWorker.join();
    if(mHSerial != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mHSerial);
        mHSerial = INVALID_HANDLE_VALUE;
    }
}

std::vector<std::string> SerialModerno::ListarPortas()
{
    std::vector<std::string> portas;
    char caminho[MAX_PATH];
    char nomePorta[16];
    for (int i = 1; i <= 255; ++i)
    {
        sprintf(nomePorta, "COM%d", i);
        // QueryDosDeviceA para bater com o char* do nomePorta
        if (QueryDosDeviceA(nomePorta, caminho, MAX_PATH) != 0) portas.push_back(nomePorta);
    }
    return portas;
}


SerialModerno::~SerialModerno(){Fechar();}

SerialModerno::SerialModerno() = default;