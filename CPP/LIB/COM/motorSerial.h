//	motorSerial.h
#pragma once
#include <QThread>
#include <vector>
#include <atomic>
#include <string>
#include "serialConfig.h"

extern "C"
{
#include <libserialport.h>
}

class MotorSerial : public QThread {
    Q_OBJECT
public:
    explicit MotorSerial(QObject *parent = nullptr);
    ~MotorSerial();
    bool abrirPorta(const std::string &nomePorta, SerialConfig::Parametros param);
    void fecharPorta();
    void enviarDados(const std::vector<uint8_t> &dados);

protected:
    void run() override;

signals:
    void dadosRecebidos(const std::vector<uint8_t> &dados);
    void erroOcorrido(const QString &msg);

private:
    struct sp_port *mPorta = nullptr;
    std::atomic<bool> mRodando{false};
};
