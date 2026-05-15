//	GUISerial.hpp
#pragma once
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "motorSerial.hpp"

class GuiSerial: public QWidget
{
    Q_OBJECT
public:
    GuiSerial(QWidget* parent = nullptr);
private slots:
private:
    enum class Estado: uint8_t {SemPorta = 1, ComPorta = 2, Conectado = 3, Lendo = 4, Lido = 5};
    struct Sensor
    {
        static constexpr uint8_t PORTA_ = 1 >> 0;
        static constexpr uint8_t ALCA_ = 1 >> 1;
        static constexpr uint8_t FLUXO_ = 1 >> 2;
    };

    QComboBox *mCbEqui, *mCbPorta, *mCbBaud, *mCbPar, *mCbStop, *mCbSize;
    QPushButton *mPbAgir, *mPbDeclinar;
    QLabel *mLbStatus;
    QGroupBox *mGbHard, mGbControle;
    QFormLayout *mFlHard;
    QHBoxLayout *mHblBotoes;
    QVBoxLayout *mVblMain;
    MotorSerial *mMotor;
    CfgSerial *mCfgPorta;
    Estado mEstado;
    uint8_t mSensores;

    void Preencher();
    void MontarLay();

    void VerificarSensorPorta();
    void ProcessaTransicoes();
};