//	motorSerial.cpp
#include "motorSerial.h"

MotorSerial::MotorSerial(QObject *parent): QThread(parent){}

MotorSerial::~MotorSerial(){fecharPorta();}

bool MotorSerial::abrirPorta(const std::string &nomePorta, SerialConfig::Parametros param)
{
    if(sp_get_port_by_name(nomePorta.c_str(), &mPorta) != SP_OK) return false;
    if(sp_open(mPorta, SP_MODE_READ_WRITE) != SP_OK)
    {
        sp_free_port(mPorta);
        return false;
    }
    sp_set_baudrate(mPorta, param.baudrate);
    sp_set_bits(mPorta, param.dataBits);
    switch (param.paridade)
    {
    case SerialConfig::Parity::Nenhuma: sp_set_parity(mPorta, SP_PARITY_NONE); break;
    case SerialConfig::Parity::Impar: sp_set_parity(mPorta, SP_PARITY_ODD); break;
    case SerialConfig::Parity::Par: sp_set_parity(mPorta, SP_PARITY_EVEN);
    }
    sp_set_stopbits(mPorta, (int)param.stopbit);
    sp_set_flowcontrol(mPorta, SP_FLOWCONTROL_NONE);
    mRodando = true;
    this->start(QThread::HighestPriority);
    return true;
}

void MotorSerial::fecharPorta()
{
    mRodando = false;
    this->wait();
    if(mPorta)
    {
        sp_close(mPorta);
        sp_free_port(mPorta);
        mPorta = nullptr;
    }
}

void MotorSerial::run()
{
    std::vector<uint8_t> bufferLeitura(1024);
    while(mRodando)
    {
        if(sp_input_waiting(mPorta) > 0)
        {
            int lidos = sp_nonblocking_read(mPorta, bufferLeitura.data(), bufferLeitura.size());
            if(lidos > 0)
                emit dadosRecebidos(std::vector<uint8_t>(bufferLeitura.begin(), bufferLeitura.begin() + lidos));
        }
        else msleep(10);
    }
}

void MotorSerial::enviarDados(const std::vector<uint8_t> &dados)
{
    if(mPorta && mRodando) sp_blocking_write(mPorta ,dados.data(), dados.size(), 100);
}