//  SerialModerno.h
#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <span>
#include "DeviceConfigs.h"
//#include "../CORE/TYPES/DeviceConfigs.h"

class SerialModerno
{
public:
    using DataCallback = std::function<void (std::span<const uint8_t>)>;
    SerialModerno();
    ~SerialModerno();
    bool Abrir(const std::string& porta, const SerialParams& params, DataCallback cb);
    void Fechar();
    bool Enviar(std::span<const uint8_t> dados);
    static std::vector<std::string> ListarPortas();
private:
    void loopLeitura();
    HANDLE mHSerial = INVALID_HANDLE_VALUE;
    std::atomic<bool> mRodando{false};
    std::thread mWorker;
    DataCallback mOnData;
};