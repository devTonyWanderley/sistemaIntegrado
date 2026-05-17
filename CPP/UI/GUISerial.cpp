//  GUISerial.cpp
#include "GUISerial.hpp"
#include <vector>
#include <QTimer>

#include <QDebug>

GuiSerial::GuiSerial(QWidget *parent): QWidget(parent)
{
    //      --Motor serial:
    mMotor = new MotorSerial;
    mCfgPorta = new CfgSerial;

    mSensores = 0;  //  pra teste
    mEstado = mEstadoAnterior = Estado::SemPorta;   //  inicializar estado

    MontarLay();

    connect(mCbEqui, QComboBox::currentIndexChanged, this, &GuiSerial::AoMudarEquipamento);
    connect(mPbAgir, QPushButton::clicked, this, &GuiSerial::AoClicarComunicar);
    connect(mPbDeclinar, &QPushButton::clicked, this, &GuiSerial::AoClicarDeclinar);

    //      --Sentinela de porta:
    QTimer *timerSensores = new QTimer(this);
    connect(timerSensores, &QTimer::timeout, this, &GuiSerial::VerificarSensorPorta);
    timerSensores->start(1500);

    VerificarSensorPorta();
}

void GuiSerial::Preencher()
{
    //  -Preencher configurações prontas:
    mCbEqui->clear();
    for(auto c : LISTA_EQUIPAMENTOS) mCbEqui->addItem(QString::fromStdString(c.nome));

    //  -Preencher baudrate:
    mCbBaud->clear();
    mCbBaud->addItems({"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"});

    //  -Preencher paridade:
    mCbPar->clear();
    mCbPar->addItems({"Nenhuma", "Impar", "Par", "Marca", "Espaço"});

    //  -Preencher bit de parada:
    mCbStop->clear();
    mCbStop->addItems({"Um", "Um e meio", "Dois"});

    //  -Tamanho:
    mCbSize->clear();
    mCbSize->addItems({"8 bits", "7 bits"});

    //  -Preencher portas, ou constatar indisponibilidade:
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
    // Lógica temporária sugerida por você:
    if(mEstado == Estado::ComPorta || mEstado == Estado::SemPorta)
    {
        if(mEstado == Estado::ComPorta && !(mSensores & Sensor::PORTA_)) mEstado = Estado::SemPorta;
        else if(mEstado == Estado::SemPorta && (mSensores & Sensor::PORTA_)) mEstado = Estado::ComPorta;
    }

    if (mEstado != mEstadoAnterior)
    {
        qDebug() << "[MUDANÇA DE ESTADO]"
                 << "Bits Sensores:" << QString::number(mSensores, 2)
                 << "| Estado Novo:" << static_cast<int>(mEstado);

        AtualizarDesignUi();
        mEstadoAnterior = mEstado;
    }
}

void GuiSerial::AtualizarDesignUi()
{
    // Estilos base para resetar as cores quando necessário
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
        mLbStatus->setStyleSheet("color: #C0392B; font-weight: bold;"); // Texto Vermelho
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
        mLbStatus->setStyleSheet("color: #2C3E50; font-weight: normal;"); // Texto Escuro Padrão
        break;

    case Estado::Conectado:
        mGbHard->setEnabled(false);
        mPbAgir->setText("Aguardando Dados...");
        mPbAgir->setEnabled(false);
        mPbAgir->setStyleSheet(btnPadrao);

        mPbDeclinar->setText("Desconectar");
        mPbDeclinar->setEnabled(true);
        mPbDeclinar->setStyleSheet(btnVermelho); // Botão Desconectar Vermelho

        mLbStatus->setText("Status: Conectado. Aguardando transmissão do equipamento...");
        mLbStatus->setStyleSheet("color: #2980B9; font-weight: bold;"); // Texto Azul
        break;

    case Estado::Lendo:
        mGbHard->setEnabled(false);
        mPbAgir->setText("Recebendo...");
        mPbAgir->setEnabled(false);
        mPbAgir->setStyleSheet(btnLaranja); // Botão de ação indica atividade

        mPbDeclinar->setText("Abortar");
        mPbDeclinar->setEnabled(true);
        mPbDeclinar->setStyleSheet(btnVermelho); // Botão Abortar Vermelho

        mLbStatus->setStyleSheet("color: #D35400; font-weight: bold;"); // Texto Laranja
        break;

    case Estado::Lido:
        mGbHard->setEnabled(false);
        mPbAgir->setText("Salvar Dados");
        mPbAgir->setEnabled(true);
        mPbAgir->setStyleSheet(btnVerde); // Botão Salvar se destaca em Verde

        mPbDeclinar->setText("Descartar");
        mPbDeclinar->setEnabled(true);
        mPbDeclinar->setStyleSheet(btnVermelho); // Botão Descartar em Vermelho

        mLbStatus->setText("Status: Transmissão concluída. Pronto para salvar.");
        mLbStatus->setStyleSheet("color: #27AE60; font-weight: bold;"); // Texto Verde
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

    qDebug() << "[SUGESTÃO APLICADA]" << LISTA_EQUIPAMENTOS[index].nome
             << "Baud:" << baudStr;
}

void GuiSerial::AoClicarComunicar()
{
    // Se estiver pronto, conecta. Se estiver lendo ou já leu, permite salvar.
    if(mEstado == Estado::ComPorta) Lincar();
    else if(mEstado == Estado::Lido || mEstado == Estado::Lendo) Salvar();
    else qDebug() << "Comportamento inesperado .. não deveria estar ativo este botão!";
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
    if(sucesso) mSensores |= Sensor::ALCA_;
    else mSensores &= ~Sensor::ALCA_;
}

void GuiSerial::Salvar()
{
    qDebug() << "[AÇÃO] Usuário finalizou/confirmou a sessão de coleta.";

    // 1. Fecha o HANDLE Win32 e encerra a thread, garantindo que o arquivo .tmp
    // descarregue os últimos bytes físicos para o disco de forma segura.
    mMotor->Fechar();

    // 2. Remove a flag de conexão e fluxo ativo da máquina de estados
    mSensores &= ~Sensor::ALCA_;
    mSensores &= ~Sensor::FLUXO_;

    // 3. Força a reavaliação física de portas do sistema e atualiza a UI
    // para retornar ao estado inicial correspondente (ComPorta ou SemPorta)
    VerificarSensorPorta();
}

void GuiSerial::Processar(std::vector<uint8_t> dd)
{
    // Altera temporariamente o estado para Lendo enquanto os dados entram
    mEstado = Estado::Lendo;

    // Ativa o sensor de fluxo de dados
    mSensores |= Sensor::FLUXO_;

    // Log estritamente numérico e limpo via Qt
    qDebug() << "[DADOS RECEBIDOS] Bytes:" << dd.size();

    // Atualiza a interface gráfica exibindo apenas o volume de dados do pacote
    mLbStatus->setText(QString("Status: Recebendo dados (%1 bytes)").arg(dd.size()));
}

void GuiSerial::TratarSinalMotor(MotorSerial::StatusEvento se)
{
    switch(se)
    {
    case MotorSerial::StatusEvento::FluxoAtivo:
        mSensores |= Sensor::FLUXO_;
        qDebug() << "[SINAL MOTOR] Fluxo de dados ficou ATIVO.";
        mLbStatus->setText("Status: Fluxo de dados ativo...");
        break;

    case MotorSerial::StatusEvento::FluxoOcioso:
        // Remove o bit de fluxo ativo, pois a porta silenciou (timeout do Win32)
        mSensores &= ~Sensor::FLUXO_;

        // Avança a máquina para o estado Lido (pronto para salvar ou fechar)
        mEstado = Estado::Lido;

        qDebug() << "[SINAL MOTOR] Fluxo de dados ficou OCIOSO. Transição para estado LIDO.";
        mLbStatus->setText("Status: Dados recebidos com sucesso. Pronto para Salvar.");

        // Força a atualização visual dos botões para o novo cenário
        AtualizarDesignUi();
        break;

    case MotorSerial::StatusEvento::AlcaPerdida:
        // Caso o cabo seja removido ou ocorra erro crítico no HANDLE Win32
        mSensores &= ~Sensor::ALCA_;
        mSensores &= ~Sensor::FLUXO_;
        mEstado = Estado::ComPorta;

        qDebug() << "[SINAL MOTOR] Alça perdida! Dispositivo desconectado.";
        mLbStatus->setText("Status: Erro! Conexão perdida com o dispositivo.");

        AtualizarDesignUi();
        break;
    }
}

void GuiSerial::AoClicarDeclinar()
{
    // Estilos base para resetar as cores quando necessário
    QString btnVerde  = "background-color: #2ECC71; color: white; font-weight: bold; border-radius: 4px;";
    QString btnVermelho = "background-color: #E74C3C; color: white; font-weight: bold; border-radius: 4px;";
    switch (mEstado)
    {
    case Estado::SemPorta:
    case Estado::ComPorta:
        // Estados iniciais: apenas fecha a aplicação
        qDebug() << "[AÇÃO] Usuário fechou a aplicação.";
        this->close();
        break;

    case Estado::Conectado:
        // Usuário desistiu antes de receber dados: fecha o motor e limpa flags
        qDebug() << "[AÇÃO] Usuário solicitou DESCONECTAR.";
        mMotor->Fechar();
        mSensores &= ~Sensor::ALCA_;

        // Força a verificação imediata para atualizar o estado correto (ComPorta)
        VerificarSensorPorta();
        break;

    case Estado::Lendo:
    case Estado::Lido:
        qDebug() << "[AÇÃO] Usuário interrompeu a sessão. Arquivo .tmp mantido para histórico.";
        mMotor->Fechar();
        mSensores &= ~Sensor::ALCA_;
        mSensores &= ~Sensor::FLUXO_;
        VerificarSensorPorta();
        break;
    }
}


