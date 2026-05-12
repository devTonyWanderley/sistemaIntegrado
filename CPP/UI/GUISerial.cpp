//  GUISerial.cpp
#include "GUISerial.hpp"

#include <QDebug>

GuiSerial::GuiSerial(QWidget *parent): QWidget(parent)
{
    MontarLay();
}

void GuiSerial::Preencher()
{
    qDebug() << "Função \'void GuiSerial::Preencher()\' - preenche as combos\n";
}

void GuiSerial::MontarLay()
{
    //  -Instanciar elementos:
    //      --Combos:
    mCbEqui = new QComboBox(this);
    mCbPorta = new QComboBox(this);
    mCbBaud = new QComboBox(this);
    mCbPar = new QComboBox(this);
    mCbStop = new QComboBox(this);
    mCbSize = new QComboBox(this);

    //      --Botões:
    mPbAgir = new QPushButton("Comunicar", this);
    mPbDeclinar = new QPushButton("Fechar", this);

    //      --Status:
    mLbStatus = new QLabel("Configure hardware e click em Comunicar", this);

    //      --Molduras:
    mGbHard = new QGroupBox("Configuração de hardware", this);
    mFlHard = new QFormLayout(mGbHard);
    mHblBotoes = new QHBoxLayout();
    mVblMain = new QVBoxLayout();

    //  -Dimensionar botões:
    mPbAgir->setMaximumHeight(50);
    mPbDeclinar->setMaximumHeight(50);

    //  -Preencher hard:
    mFlHard->addRow("Equipamento: ", mCbEqui);
    mFlHard->addRow("Porta: ", mCbPorta);
    mFlHard->addRow("Baudrate: ", mCbBaud);
    mFlHard->addRow("Paridade: ", mCbPar);
    mFlHard->addRow("Bit de parada: ", mCbStop);
    mFlHard->addRow("Tamanho: ", mCbSize);

    //  -Preencher controle:
    mHblBotoes->addWidget(mPbAgir, 1);
    mHblBotoes->addWidget(mPbDeclinar, 1);

    //  -Preencher o layout principal:
    mVblMain->addWidget(mGbHard);
    mVblMain->addLayout(mHblBotoes);
    mVblMain->addWidget(mLbStatus);

    this->setLayout(mVblMain);
    this->resize(400,300);

    Preencher();
}