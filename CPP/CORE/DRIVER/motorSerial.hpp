//	motorSerial.hpp .. versão atualizada de "SerialModerno"
#pragma once
#include "configSerial.hpp"
#include <functional>
#include <span>
#include <vector>
#include <string>
#include <windows.h>
#include <atomic>
#include <thread>
#include <fstream>

class MotorSerial
{
public:
    using DataCallback = std::function<void (std::span<const uint8_t>)>;
    MotorSerial();
    ~MotorSerial();
    static std::vector<std::string> ListarPortas();
    bool Abrir(const std::string& porta, const CfgSerial& params, DataCallback cb);
    bool Enviar(std::span<const uint8_t> dados);
    void Fechar();
private:
    HANDLE mHSerial = INVALID_HANDLE_VALUE;
    std::atomic<bool> mRodando{false};
    std::thread mWorker;
    DataCallback mOnData;
    std::string mNomeArquivo;
    std::ofstream mArquivo;
    void LoopLeitura();
    std::string GerarNomeTmp() const;
};