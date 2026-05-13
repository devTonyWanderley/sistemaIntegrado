//  GUISerial.cpp
#include "GUISerial.hpp"
#include <vector>

#include <QDebug>

GuiSerial::GuiSerial(QWidget *parent): QWidget(parent)
{
    //      --Motor serial:
    mMotor = new MotorSerial;
    mCfgPorta = new CfgSerial;
    mEstado = 0;

    MontarLay();
}

void GuiSerial::Preencher()
{
    //  -Preencher configurações prontas:
    mCbEqui->clear();
    for(auto c : LISTA_EQUIPAMENTOS) mCbEqui->addItem(QString::fromStdString(c.nome));

    //  -Preencher portas, ou constatar indisponibilidade:
    mCbPorta->clear();
    std::vector<std::string> portas = mMotor->ListarPortas();
    if(portas.empty())
    {
        mCbPorta->addItem("Nenhuma");
        mEstado &= 0xfffffffe;
    }
    else
    {
        for(std::string p : portas) mCbPorta->addItem(QString::fromStdString(p));
        mEstado |= 1;
    }

    //  -Preencher baudrate:
    mCbBaud->clear();
    mCbBaud->addItem("1200");
    mCbBaud->addItem("2400");
    mCbBaud->addItem("4800");
    mCbBaud->addItem("9600");
    mCbBaud->addItem("19200");
    mCbBaud->addItem("38400");
    mCbBaud->addItem("57600");
    mCbBaud->addItem("115200");

    //  -Preencher paridade:
    mCbPar->clear();
    mCbPar->addItem("Nenhuma");
    mCbPar->addItem("Impar");
    mCbPar->addItem("Par");
    mCbPar->addItem("Marca");
    mCbPar->addItem("Espaço");

    //  -Preencher bit de parada:
    mCbStop->clear();
    mCbStop->addItem("Um");
    mCbStop->addItem("Um e meio");
    mCbStop->addItem("Dois");

    //  -Tamanho:
    mCbSize->clear();
    mCbSize->addItem("8 bits");
    mCbSize->addItem("7 bits");
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