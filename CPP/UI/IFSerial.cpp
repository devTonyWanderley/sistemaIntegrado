//  IFSerial.cpp
#include "../UI/IFSerial.h"
#include "../CORE/TYPES/Equipamentos.h"
#include <windows.h>
#include <QString>
#include <QFileDialog>
#include <fstream>

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
    mGrupoHard = new QGroupBox("Configuração de Hardware", this);
    mLayHard = new QFormLayout(mGrupoHard);
    mLayBotoes = new QHBoxLayout();
    mLayPrincipal = new QVBoxLayout();
    Povoar();   //  povoar combobox
    MontarLayout(); //  montar layout
    connect(mCbEquipo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IFSerial::onCbEquipoChanged);
    connect(mPbAgir, &QPushButton::clicked, this, &IFSerial::onPbAgirClicked);
    connect(mPbDeclinar, &QPushButton::clicked, this, &IFSerial::onPbDeclinarClicked);
}

QStringList IFSerial::buscaPortas()
{
    std::vector<std::string> strPortas = SerialModerno::ListarPortas();
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
    mCbEquipo->clear();
    for(auto &e : LISTA_EQUIPAMENTOS) mCbEquipo->addItem(e.nome);
    mCbPorta->clear();
    QStringList listPortas = buscaPortas();
    if(listPortas.empty()) mCbPorta->addItem("Null");
    else for(auto &nome : listPortas) mCbPorta->addItem(nome);
    mCbBaud->clear();
    mCbBaud->addItem("2400");
    mCbBaud->addItem("4800");
    mCbBaud->addItem("9600");
    mCbBaud->addItem("19200");
    mCbBaud->addItem("38400");
    mCbBaud->addItem("57600");
    mCbBaud->addItem("115200");
    mCbPar->clear();
    mCbPar->addItem("Nenhuma");
    mCbPar->addItem("Impar");
    mCbPar->addItem("Par");
    mCbStop->clear();
    mCbStop->addItem("Um");
    mCbStop->addItem("Um e meio");
    mCbStop->addItem("Dois");
}

void IFSerial::MontarLayout()
{
    mLayHard->addRow("Equipo: ", mCbEquipo);
    mLayHard->addRow("Porta: ", mCbPorta);
    mLayHard->addRow("Baudrate: ", mCbBaud);
    mLayHard->addRow("Paridade: ", mCbPar);
    mLayHard->addRow("Parada: ", mCbStop);
    mLayBotoes->addWidget(mPbAgir, 1);
    mLayBotoes->addWidget(mPbDeclinar, 1);
    mLayPrincipal->addWidget(mGrupoHard);
    mLayPrincipal->addLayout(mLayBotoes);
    mLayPrincipal->addWidget(mLbStatus);
    this->setLayout(mLayPrincipal);
}

void IFSerial::AtualizarInterface()
{
    bool podeEditarHardwere = (mFase == Fase::CONECTANDO);
    mGrupoHard->setEnabled(podeEditarHardwere);
    switch(mFase)
    {
    case Fase::CONECTANDO:
        mPbAgir->setText("Comunicar");
        mPbAgir->setStyleSheet("background-color: #2ecc71; color: white; font-weight: bold;");
        mPbDeclinar->setText("Sair");
        mLbStatus->setText("Status: Pronto para conectar");
        break;
    case Fase::CAPTURANDO:
        mPbAgir->setText("Finalizar Captura");
        mPbAgir->setStyleSheet("background-color: #f1c40f; color: black; font-weight: bold;");
        mPbDeclinar->setText("Abortar");
        mPbDeclinar->setStyleSheet("background-color: #e74c3c; color: white;");
        mLbStatus->setText("Status: Recebendo dados...");
        break;
    case Fase::SALVANDO:
        mPbAgir->setText("Salvar Dados (STL)");
        mPbAgir->setStyleSheet("background-color: #3498db; color: white; font-weight: bold;");
        mPbDeclinar->setText("Novo");
        mLbStatus->setText("Status: Captura finalizada. Aguardando salvamento.");
    }
}

void IFSerial::onPbAgirClicked()
{
    switch (mFase)
    {
    case Fase::CONECTANDO:
        if(TentarAbrirConexao())
        {
            mFase = Fase::CAPTURANDO;
            mEstado = Status::OCUPADO;
        }
        break;
    case Fase::CAPTURANDO:
        mMotor.Fechar();
        mFase = Fase::SALVANDO;
        mEstado = Status::OCIOSO;
        break;
    case Fase::SALVANDO:
        if(SalvarArquivoSTL())
        {
            mEspelhoCabo.clear();
            mFase = Fase::CONECTANDO;
        }
    }
    AtualizarInterface();
}

bool IFSerial::TentarAbrirConexao()
{
    QString portaNome = mCbPorta->currentText();
    if(portaNome == "Null" || portaNome.isEmpty())
    {
        mLbStatus->setText("Status: Nenhuma porta selecionada!");
        return false;
    }
    uint32_t baud = mCbBaud->currentText().toUInt();
    uint8_t paridade = static_cast<uint8_t>(mCbPar->currentIndex()); // 0:None, 1:Odd, 2:Even
    uint8_t stopBits = static_cast<uint8_t>(mCbStop->currentIndex()); // 0:1, 1:1.5, 2:2
    uint8_t dataSize = 8;
    SerialParams params = { .raw = SerialParams::build(baud, paridade, stopBits, dataSize) };
    mEspelhoCabo.clear();
    bool sucesso = mMotor.Abrir(portaNome.toStdString(), params, [this](std::span<const uint8_t> bloco)
                                {std::vector<char> dadosCopia(bloco.begin(), bloco.end());
                                    QMetaObject::invokeMethod(this, [this, dadosCopia = std::move(dadosCopia)]() mutable
                                                              {
                                                                  mEspelhoCabo.insert(mEspelhoCabo.end(), dadosCopia.begin(), dadosCopia.end());
                                                                  mLbStatus->setText(QString("Capturando: %1 bytes recebidos...")
                                                                                         .arg(mEspelhoCabo.size()));
                                                              }, Qt::QueuedConnection);
                                });
    if(!sucesso)
    {
        mLbStatus->setText("Status: Falha ao abrir " + portaNome + " (Verifique se está em uso)");
        return false;
    }
    mLbStatus->setText("Status: Conectado a " + portaNome);
    return true;
}

bool IFSerial::SalvarArquivoSTL()
{
    QString caminho = QFileDialog::getSaveFileName(this, "Salvar Dados", "", "ecr (*.ecr)");
    if(caminho.isEmpty()) return false;
    std::ofstream arquivo(caminho.toStdString(), std::ios::binary);
    if(!arquivo.is_open()) return false;
    arquivo.write(reinterpret_cast<const char*>(mEspelhoCabo.data()), mEspelhoCabo.size());
    arquivo.close();
    return true;
}

void IFSerial::onCbEquipoChanged(int index)
{
    if(index < 0) return;
    SerialParams p = { .raw = LISTA_EQUIPAMENTOS[index].params };
    mCbBaud->setCurrentText(QString::number(p.winBaud()));
    mCbPar->setCurrentIndex(p.winParity());
    mCbStop->setCurrentIndex(p.winStop());
    mLbStatus->setText(QString("Configuração sugerida para %1 carregada.").arg(LISTA_EQUIPAMENTOS[index].nome));
}

void IFSerial::onPbDeclinarClicked()
{
    switch (mFase)
    {
    case Fase::CONECTANDO:
        this->close();
        break;
    case Fase::CAPTURANDO:
        mMotor.Fechar();
        mEspelhoCabo.clear();
        mFase = Fase::CONECTANDO;
        mLbStatus->setText("Status: Captura abortada. Dados descartados.");
        break;
    case Fase::SALVANDO:
        mEspelhoCabo.clear();
        mFase = Fase::CONECTANDO;
        mLbStatus->setText("Status: Dados descartados. Pronto para nova leitura.");
        break;
    }
    Povoar();
    AtualizarInterface();
}
