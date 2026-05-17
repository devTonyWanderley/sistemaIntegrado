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
    enum class StatusEvento{AlcaPerdida = 0, FluxoAtivo = 1, FluxoOcioso = 2};
    using DataCallback = std::function<void (std::span<const uint8_t>)>;
    using StatusCallback = std::function<void (StatusEvento)>;
    MotorSerial();
    ~MotorSerial();
    static std::vector<std::string> ListarPortas();
    bool Abrir(const std::string& porta, const CfgSerial& params, DataCallback cb, StatusCallback cbStatus);
    bool Enviar(std::span<const uint8_t> dados);
    void Fechar();
    std::string ObterPathFile() const {return mNomeArquivo;}
private:
    HANDLE mHSerial = INVALID_HANDLE_VALUE;
    std::atomic<bool> mRodando{false};
    std::thread mWorker;
    DataCallback mOnData;
    StatusCallback mOnStatus;
    std::string mNomeArquivo;
    std::ofstream mArquivo;
    void LoopLeitura();
    std::string GerarNomeTmp() const;
};