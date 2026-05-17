//  GUISerial.cpp
#include "GUISerial.hpp"
#include <vector>
#include <QTimer>
#include <filesystem>

GuiSerial::GuiSerial(QWidget *parent): QWidget(parent)
{
    mMotor = new MotorSerial;
    mCfgPorta = new CfgSerial;
    mEstado = mEstadoAnterior = Estado::SemPorta;
    MontarLay();
    if (mCbEqui->count() > 0)
    {
        mCbEqui->setCurrentIndex(0);
        AoMudarEquipamento(0);
    }
    connect(mCbEqui, QComboBox::currentIndexChanged, this, &GuiSerial::AoMudarEquipamento);
    connect(mPbAgir, QPushButton::clicked, this, &GuiSerial::AoClicarComunicar);
    connect(mPbDeclinar, &QPushButton::clicked, this, &GuiSerial::AoClicarDeclinar);
    QTimer *timerSensores = new QTimer(this);
    connect(timerSensores, &QTimer::timeout, this, &GuiSerial::VerificarSensorPorta);
    timerSensores->start(1500);
    VerificarSensorPorta();
}

void GuiSerial::Preencher()
{
    mCbEqui->clear();
    for(auto c : LISTA_EQUIPAMENTOS) mCbEqui->addItem(QString::fromStdString(c.nome));
    mCbBaud->clear();
    mCbBaud->addItems({"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"});
    mCbPar->clear();
    mCbPar->addItems({"Nenhuma", "Impar", "Par", "Marca", "Espaço"});
    mCbStop->clear();
    mCbStop->addItems({"Um", "Um e meio", "Dois"});
    mCbSize->clear();
    mCbSize->addItems({"8 bits", "7 bits"});
    mCbPorta->clear();
    std::vector<std::string> portas = mMotor->ListarPortas();
    for(const std::string &p : portas) mCbPorta->addItem(QString::fromStdString(p));
    if(portas.empty())
    {
        mSensores &= ~Sensor::PORTA_;
        mEstado = Estado::SemPorta;
    }
    else
    {
        mSensores |= Sensor::PORTA_;
        mEstado = Estado::ComPorta;
    }
    mEstadoAnterior = mEstado;
    AtualizarDesignUi();
}

void GuiSerial::MontarLay()
{
    mCbEqui = new QComboBox(this);
    mCbPorta = new QComboBox(this);
    mCbBaud = new QComboBox(this);
    mCbPar = new QComboBox(this);
    mCbStop = new QComboBox(this);
    mCbSize = new QComboBox(this);
    mPbAgir = new QPushButton("Comunicar", this);
    mPbDeclinar = new QPushButton("Fechar", this);
    mLbStatus = new QLabel("Configure hardware e click em Comunicar", this);
    mGbHard = new QGroupBox("Configuração de hardware", this);
    mFlHard = new QFormLayout(mGbHard);
    mHblBotoes = new QHBoxLayout();
    mVblMain = new QVBoxLayout();
    mPbAgir->setMaximumHeight(50);
    mPbDeclinar->setMaximumHeight(50);
    mFlHard->addRow("Equipamento: ", mCbEqui);
    mFlHard->addRow("Porta: ", mCbPorta);
    mFlHard->addRow("Baudrate: ", mCbBaud);
    mFlHard->addRow("Paridade: ", mCbPar);
    mFlHard->addRow("Bit de parada: ", mCbStop);
    mFlHard->addRow("Tamanho: ", mCbSize);
    mHblBotoes->addWidget(mPbAgir, 1);
    mHblBotoes->addWidget(mPbDeclinar, 1);
    mVblMain->addWidget(mGbHard);
    mVblMain->addLayout(mHblBotoes);
    mVblMain->addWidget(mLbStatus);
    this->setLayout(mVblMain);
    this->resize(400,300);
    Preencher();
}

void GuiSerial::VerificarSensorPorta()
{
    std::vector<std::string> portas = MotorSerial::ListarPortas();
    uint8_t sensoresAnterior = mSensores;
    if(portas.empty()) mSensores &= ~Sensor::PORTA_;
    else mSensores |= Sensor::PORTA_;
    if((mSensores & Sensor::PORTA_) != (sensoresAnterior & Sensor::PORTA_) || static_cast<int>(portas.size()) != mCbPorta->count())
    {
        mCbPorta->clear();
        for(const std::string &p : portas) mCbPorta->addItem(QString::fromStdString(p));
    }
    ProcessaTransicoes();
}

void GuiSerial::ProcessaTransicoes()
{
    if(mEstado == Estado::ComPorta || mEstado == Estado::SemPorta)
    {
        if(mEstado == Estado::ComPorta && !(mSensores & Sensor::PORTA_)) mEstado = Estado::SemPorta;
        else if(mEstado == Estado::SemPorta && (mSensores & Sensor::PORTA_)) mEstado = Estado::ComPorta;
    }
    else if(!(mSensores & Sensor::ALCA_)) mEstado = (mSensores & Sensor::PORTA_) ? Estado::ComPorta : Estado::SemPorta;
    if (mEstado != mEstadoAnterior)
    {
        AtualizarDesignUi();
        mEstadoAnterior = mEstado;
    }
}

void GuiSerial::AtualizarDesignUi()
{
    QString btnPadrao = "background-color: none; color: none; font-weight: normal;";
    QString btnVerde  = "background-color: #2ECC71; color: white; font-weight: bold; border-radius: 4px;";
    QString btnVermelho = "background-color: #E74C3C; color: white; font-weight: bold; border-radius: 4px;";
    QString btnLaranja  = "background-color: #E67E22; color: white; font-weight: bold; border-radius: 4px;";
    switch (mEstado)
    {
    case Estado::SemPorta:
        mGbHard->setEnabled(false);
        mPbAgir->setText("Conectar");
        mPbAgir->setEnabled(false);
        mPbAgir->setStyleSheet(btnPadrao);
        mPbDeclinar->setText("Fechar");
        mPbDeclinar->setEnabled(true);
        mPbDeclinar->setStyleSheet(btnPadrao);
        mLbStatus->setText("Status: Nenhuma porta COM disponível");
        mLbStatus->setStyleSheet("color: #C0392B; font-weight: bold;");
        break;
    case Estado::ComPorta:
        mGbHard->setEnabled(true);
        mPbAgir->setText("Conectar");
        mPbAgir->setEnabled(true);
        mPbAgir->setStyleSheet(btnPadrao);
        mPbDeclinar->setText("Fechar");
        mPbDeclinar->setEnabled(true);
        mPbDeclinar->setStyleSheet(btnPadrao);
        mLbStatus->setText("Status: Pronto para conectar");
        mLbStatus->setStyleSheet("color: #2C3E50; font-weight: normal;");
        break;
    case Estado::Conectado:
        mGbHard->setEnabled(false);
        mPbAgir->setText("Aguardando Dados...");
        mPbAgir->setEnabled(false);
        mPbAgir->setStyleSheet(btnPadrao);
        mPbDeclinar->setText("Desconectar");
        mPbDeclinar->setEnabled(true);
        mPbDeclinar->setStyleSheet(btnVermelho);
        mLbStatus->setText("Status: Conectado. Aguardando transmissão do equipamento...");
        mLbStatus->setStyleSheet("color: #2980B9; font-weight: bold;");
        break;
    case Estado::Lendo:
        mGbHard->setEnabled(false);
        mPbAgir->setText("Recebendo...");
        mPbAgir->setEnabled(false);
        mPbAgir->setStyleSheet(btnLaranja);
        mPbDeclinar->setText("Abortar");
        mPbDeclinar->setEnabled(true);
        mPbDeclinar->setStyleSheet(btnVermelho);
        mLbStatus->setStyleSheet("color: #D35400; font-weight: bold;");
        break;
    case Estado::Lido:
        mGbHard->setEnabled(false);
        mPbAgir->setText("Salvar Dados");
        mPbAgir->setEnabled(true);
        mPbAgir->setStyleSheet(btnVerde);
        mPbDeclinar->setText("Descartar");
        mPbDeclinar->setEnabled(true);
        mPbDeclinar->setStyleSheet(btnVermelho);
        mLbStatus->setText("Status: Transmissão concluída. Pronto para salvar.");
        mLbStatus->setStyleSheet("color: #27AE60; font-weight: bold;");
        break;
    }
}

void GuiSerial::AoMudarEquipamento(int index)
{
    if(index < 0 || index >= static_cast<int>(sizeof(LISTA_EQUIPAMENTOS) / sizeof(PerfilEquipamento))) return;
    CfgSerial sugestao;
    sugestao.raw = LISTA_EQUIPAMENTOS[index].params;
    QString baudStr = QString::number(sugestao.winBaud());
    int baudIdx = mCbBaud->findText(baudStr);
    if(baudIdx != -1) mCbBaud->setCurrentIndex(baudIdx);
    uint8_t paridade = sugestao.winParity(), stopBits = sugestao.winStop(), tamanho = sugestao.winSize();
    if(paridade < mCbPar->count()) mCbPar->setCurrentIndex(paridade);
    if(stopBits < mCbStop->count()) mCbStop->setCurrentIndex(stopBits);
    if(tamanho == 7) mCbSize->setCurrentIndex(1);
    else mCbSize->setCurrentIndex(0);
}

void GuiSerial::AoClicarComunicar()
{
    if(mEstado == Estado::ComPorta)
    {
        mLbStatus->setText("Aguardando dados...");
        Lincar();
    }
    else if(mEstado == Estado::Lido || mEstado == Estado::Lendo) Salvar();
}

void GuiSerial::Lincar()
{
    if(mCbPorta->currentText().isEmpty()) return;
    uint32_t baud = mCbBaud->currentText().toUInt();
    uint8_t
        paridade = static_cast<uint8_t>(mCbPar->currentIndex()),
        stopBit = static_cast<uint8_t>(mCbStop->currentIndex()),
        tamanho = (mCbSize->currentIndex() == 0) ? 8 : 7;
    mCfgPorta->raw = CfgSerial::build(baud, paridade, stopBit, tamanho);
    std::string porta = mCbPorta->currentText().toStdString();
    bool sucesso = mMotor->Abrir(
        porta, *mCfgPorta,
        [this](std::span<const uint8_t> d) {
            QMetaObject::invokeMethod(this, [this, dados = std::vector<uint8_t>(d.begin(), d.end())]() {
                this->Processar(dados);
            }, Qt::QueuedConnection);
        },
        [this](MotorSerial::StatusEvento ev) {
            QMetaObject::invokeMethod(this, [this, ev]() {
                this->TratarSinalMotor(ev);
            }, Qt::QueuedConnection);
        }
        );
    if(sucesso)
    {
        mSensores |= Sensor::ALCA_;
        ProcessaTransicoes();
    }
    else
    {
        mSensores &= ~Sensor::ALCA_;
        mLbStatus->setText("Status: Falha ao abrir a porta selecionada.");
        mLbStatus->setStyleSheet("color: #C0392B; font-weight: bold;");
    }
}

void GuiSerial::Salvar()
{
    mMotor->Fechar();
    mSensores &= ~Sensor::ALCA_;
    mSensores &= ~Sensor::FLUXO_;
    VerificarSensorPorta();
}

void GuiSerial::Processar(std::vector<uint8_t> dd)
{
    mEstado = Estado::Lendo;
    mSensores |= Sensor::FLUXO_;
    mLbStatus->setText(QString("Status: Recebendo dados (%1 bytes)").arg(dd.size()));
}

void GuiSerial::TratarSinalMotor(MotorSerial::StatusEvento se)
{
    switch(se)
    {
    case MotorSerial::StatusEvento::FluxoAtivo:
        mSensores |= Sensor::FLUXO_;
        mLbStatus->setText("Status: Fluxo de dados ativo...");
        break;
    case MotorSerial::StatusEvento::FluxoOcioso:
        mSensores &= ~Sensor::FLUXO_;
        mEstado = Estado::Lido;
        mLbStatus->setText("Status: Dados recebidos com sucesso. Pronto para Salvar.");
        AtualizarDesignUi();
        break;
    case MotorSerial::StatusEvento::AlcaPerdida:
        mSensores &= ~Sensor::ALCA_;
        mSensores &= ~Sensor::FLUXO_;
        mEstado = Estado::ComPorta;
        mLbStatus->setText("Status: Erro! Conexão perdida com o dispositivo.");
        AtualizarDesignUi();
        break;
    }
}

void GuiSerial::AoClicarDeclinar()
{
    switch (mEstado)
    {
    case Estado::SemPorta:
    case Estado::ComPorta:
        this->close();
        break;
    case Estado::Conectado:
        mMotor->Fechar();
        mSensores &= ~Sensor::ALCA_;
        VerificarSensorPorta();
        break;
    case Estado::Lendo:
    case Estado::Lido:
        std::string nome = mMotor->ObterPathFile();
        mMotor->Fechar();
        mSensores &= ~Sensor::ALCA_;
        mSensores &= ~Sensor::FLUXO_;
        if (!nome.empty() && std::filesystem::exists(nome)) std::filesystem::remove(nome);
        VerificarSensorPorta();
        break;
    }
}