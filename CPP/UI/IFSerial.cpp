#include "../UI/IFSerial.h"
#include "../CORE/TYPES/Equipamentos.h"
#include "../CORE/DRIVER/SerialWin32.h"
#include <windows.h>
#include <QString>

#include <QDebug>

IFSerial::IFSerial(QWidget *parent): QWidget(parent), mEstado(Status::OCIOSO), mFase(Fase::CONECTANDO)
{
    //  Instanciar objetos:
    mLbStatus = new QLabel(this);

    mCbEquipo = new QComboBox(this);
    mCbPorta = new QComboBox(this);
    mCbBaud = new QComboBox(this);
    mCbPar = new QComboBox(this);
    mCbStop = new QComboBox(this);

    mPbAgir = new QPushButton("Comunicar", this);
    mPbAgir->setMaximumHeight(50);
    mPbDeclinar = new QPushButton("Declinar", this);
    mPbDeclinar->setMaximumHeight(50);
    mPbAtualizar = new QPushButton("Atualizar", this);
    mPbAtualizar->setMaximumHeight(50);

    mGrupoHard = new QGroupBox("Configuração de Hardware", this);
    mLayHard = new QFormLayout(mGrupoHard);
    mLayBotoes = new QHBoxLayout();
    mLayPrincipal = new QVBoxLayout();

    Povoar();   //  povoar combobox

    MontarLayout(); //  montar layout
    //  [...]
}

QStringList IFSerial::buscaPortas()
{
    std::vector<std::string> strPortas = SerialWin32::listarPortas();
    QStringList r;
    if (!strPortas.empty())
    {
        r.reserve(static_cast<qsizetype>(strPortas.size()));
        for (const std::string &tx : strPortas) r << QString::fromStdString(tx);
    }
    return r;
}

void IFSerial::Povoar()
{
    //  povoar mCbEquipo:
    mCbEquipo->clear();
    for(auto &e : LISTA_EQUIPAMENTOS) mCbEquipo->addItem(e.nome);

    //  povoar mCbPorta:
    mCbPorta->clear();
    QStringList listPortas = buscaPortas();
    if(listPortas.empty()) mCbPorta->addItem("Null");
    else for(auto &nome : listPortas) mCbPorta->addItem(nome);

    //  povoar mCbBaud:
    mCbBaud->clear();
    mCbBaud->addItem("2400");
    mCbBaud->addItem("4800");
    mCbBaud->addItem("9600");
    mCbBaud->addItem("19200");
    mCbBaud->addItem("38400");
    mCbBaud->addItem("57600");
    mCbBaud->addItem("115200");

    //  povoar mCbPar:
    mCbPar->clear();
    mCbPar->addItem("Nenhuma");
    mCbPar->addItem("Impar");
    mCbPar->addItem("Par");

    //  povoar mCbStop:
    mCbStop->clear();
    mCbStop->addItem("Um");
    mCbStop->addItem("Um e meio");
    mCbStop->addItem("Dois");
    //  [...]?  verificar se precisa habilitar aqui os comobobox's
}

void IFSerial::MontarLayout()
{
    mLayHard->addRow("Equipo: ", mCbEquipo);
    mLayHard->addRow("Porta: ", mCbPorta);
    mLayHard->addRow("Baudrate: ", mCbBaud);
    mLayHard->addRow("Paridade: ", mCbPar);
    mLayHard->addRow("Parada: ", mCbStop);

    mLayBotoes->addWidget(mPbAgir, 2);
    mLayBotoes->addWidget(mPbDeclinar, 2);
    mLayBotoes->addWidget(mPbAtualizar, 1);

    mLayPrincipal->addWidget(mGrupoHard);
    mLayPrincipal->addLayout(mLayBotoes);
    mLayPrincipal->addWidget(mLbStatus);

    this->setLayout(mLayPrincipal);
}