//	main.cpp
#include <QCoreApplication>
#include <QDebug>
#include "../../LIB/COM/motorSerial.h"
#include "../../LIB/TIP/serialConfig.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "--- Teste de Hardware Serial (C++20) ---";
    MotorSerial motor;
    QObject::connect(&motor, &MotorSerial::dadosRecebidos, [](const std::vector<uint8_t> &dados)
    {
        QString str;
        for(auto b : dados) str += QString::asprintf("%02X ", b);
        qDebug() << "RX [HEX]:" << str;
    });
    SerialConfig::Parametros params {
        .baudrate = 9600,
        .dataBits = 8,
        .paridade = SerialConfig::Parity::Nenhuma,
        .stopbit = SerialConfig::StopBit::Um
    };
    if(motor.abrirPorta("COM1", params))
    {
        qDebug() << "Porta aberta com sucesso. Aguardando dados...";
        std::vector<uint8_t> ola = {0x4F, 0x4C, 0x41, 0x0D, 0x0A};
        motor.enviarDados(ola);
    }
    else qDebug() << "Falha ao abrir porta. Verifique se o dispositivo está conectado.";
    return a.exec();
}