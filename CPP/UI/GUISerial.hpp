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
    QComboBox *mCbEqui, *mCbPorta, *mCbBaud, *mCbPar, *mCbStop, *mCbSize;
    QPushButton *mPbAgir, *mPbDeclinar;
    QLabel *mLbStatus;
    QGroupBox *mGbHard, mGbControle;
    QFormLayout *mFlHard;
    QHBoxLayout *mHblBotoes;
    QVBoxLayout *mVblMain;
    MotorSerial *mMotor;
    CfgSerial *mCfgPorta;
    uint32_t mEstado;

    void Preencher();
    void MontarLay();
};