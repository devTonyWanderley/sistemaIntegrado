//	motorSerial.cpp .. versão atualizada de "SerialModerno"
#include "motorSerial.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

const std::string PATH_TRABALHO = "C:/DESENV/CPP/INSTANCIA/TMP/";

MotorSerial::MotorSerial() = default;

MotorSerial::~MotorSerial()
{
    Fechar();
}

std::vector<std::string> MotorSerial::ListarPortas()
{
    std::vector<std::string> portas;
    char caminho[MAX_PATH];
    char nomePorta[16];
    for(auto i = 0; i < 256; i++)
    {
        sprintf(nomePorta, "COM%d", i);
        if (QueryDosDeviceA(nomePorta, caminho, MAX_PATH) != 0) portas.push_back(nomePorta);
    }
    return portas;
}

bool MotorSerial::Abrir(const std::string &porta, const CfgSerial &params, DataCallback cb)
{
    mHSerial = CreateFileA(
        porta.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
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

    mNomeArquivo = GerarNomeTmp();

    COMMTIMEOUTS timeouts = {};
    //timeouts.ReadIntervalTimeout = MAXWORD;
    timeouts.ReadIntervalTimeout = 50;

    //timeouts.ReadTotalTimeoutConstant = 50;    //  alterado de 0 pra 50
    uint32_t baud = params.winBaud();
    timeouts.ReadTotalTimeoutConstant = (baud > 0) ? (200000 / baud) + 100 : 500;

    timeouts.ReadTotalTimeoutMultiplier = 0;
    SetCommTimeouts(mHSerial, &timeouts);
    mRodando = true;
    mWorker = std::thread(&MotorSerial::LoopLeitura, this);
    return true;
}

bool MotorSerial::Enviar(std::span<const uint8_t> dados)
{
    if(mHSerial == INVALID_HANDLE_VALUE) return false;
    DWORD bytesEscritos = 0;
    if(WriteFile(mHSerial, dados.data(), static_cast<DWORD>(dados.size()), &bytesEscritos, NULL))
        return bytesEscritos == dados.size();
    return false;
}

void MotorSerial::Fechar()
{
    mRodando = false;
    if(mWorker.joinable()) mWorker.join();

    if(mArquivo.is_open()) mArquivo.close();

    if(mHSerial != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mHSerial);
        mHSerial = INVALID_HANDLE_VALUE;
    }
}

void MotorSerial::LoopLeitura()
{
    uint8_t buffer[512];
    DWORD bytesLidos = 0;
    while(mRodando)
    {
        if(!ReadFile(mHSerial, buffer, sizeof(buffer), &bytesLidos, NULL))
        {
            DWORD erro = GetLastError();
            if(erro == ERROR_ACCESS_DENIED || erro == ERROR_INVALID_HANDLE || erro == ERROR_DEVICE_NOT_CONNECTED)
            {
                mRodando = false;
                break;
            }
            continue;
        }
        if(bytesLidos > 0 && mOnData)
        {
            if(!mArquivo.is_open()) mArquivo.open(mNomeArquivo, std::ios::app | std::ios::binary);
            if(mArquivo.is_open())
            {
                mArquivo.write(reinterpret_cast<const char*>(buffer), bytesLidos);
                mArquivo.flush();
            }
            mOnData(std::span<const uint8_t>(buffer, bytesLidos));
        }
        if(bytesLidos == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

std::string MotorSerial::GerarNomeTmp() const
{
    auto agora = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm bt{};
    localtime_s(&bt, &agora);
    std::stringstream ss;
    ss << PATH_TRABALHO << std::put_time(&bt, "%Y%m%d%H%M%S") << ".tmp";
    return ss.str();
}