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
    mPbAtualizar = new QPushButton("Atualizar", this);
    mPbAtualizar->setMaximumHeight(50);

    mGrupoHard = new QGroupBox("Configuração de Hardware", this);
    mLayHard = new QFormLayout(mGrupoHard);
    mLayBotoes = new QHBoxLayout();
    mLayPrincipal = new QVBoxLayout();

    Povoar();   //  povoar combobox

    MontarLayout(); //  montar layout

    // 1. Quando mudar o equipamento, atualiza os campos de hardware
    connect(mCbEquipo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &IFSerial::onCbEquipoChanged);

    // 2. O botão principal (Agir) que controla a Máquina de Estados
    connect(mPbAgir, &QPushButton::clicked, this, &IFSerial::onPbAgirClicked);

    // 3. O botão de escape (Declinar) que cancela ou sai
    connect(mPbDeclinar, &QPushButton::clicked, this, &IFSerial::onPbDeclinarClicked);

    //  [...]
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

void IFSerial::AtualizarInterface()
{
    bool podeEditarHardwere = (mFase == Fase::CONECTANDO);
    mGrupoHard->setEnabled(podeEditarHardwere);
    mPbAtualizar->setEnabled(podeEditarHardwere);
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
    QString porta = mCbPorta->currentText();
    if(porta == "Null" || porta.isEmpty())
    {
        mLbStatus->setText("Status: Nenhuma porta válida selecionada!");
        return false;
    }
    uint32_t baud = mCbBaud->currentText().toUInt();
    uint8_t paridade = static_cast<uint8_t>(mCbPar->currentIndex());
    uint8_t stopBits = static_cast<uint8_t>(mCbStop->currentIndex());
    uint8_t dataSize = 8;
    SerialParams params = { .raw = SerialParams::build(baud, paridade, stopBits, dataSize) };
    mEspelhoCabo.clear();
    bool sucesso = mMotor.Abrir(porta.toStdString(), params, [this](std::span<const uint8_t> bloco)
    {
        QMetaObject::invokeMethod(this, [this, bloco]()
        {
            mEspelhoCabo.insert(mEspelhoCabo.end(), bloco.begin(), bloco.end());
            mLbStatus->setText(QString("Capturando: %1 bytes recebidos...").arg(mEspelhoCabo.size()));
        },
        Qt::QueuedConnection);
    });
    if(!sucesso)
    {
        mLbStatus->setText("Status: Falha ao abrir " + porta);
        return false;
    }
    return true;
}

bool IFSerial::SalvarArquivoSTL()
{
    QString caminho = QFileDialog::getSaveFileName(this, "Salvar Dados", "", "ecr (*.ecr)");
    if(caminho.isEmpty()) return false;
    std::ofstream arquivo(caminho.toStdString(), std::ios::binary);
    if(!arquivo.is_open()) return false;
    arquivo.write(mEspelhoCabo.data(), mEspelhoCabo.size());
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
        // No estado inicial, o botão serve para fechar o programa
        this->close();
        break;

    case Fase::CAPTURANDO:
        // Se estiver lendo o cabo, interrompe o motor e joga o lixo fora
        mMotor.Fechar();
        mEspelhoCabo.clear();
        mFase = Fase::CONECTANDO;
        mLbStatus->setText("Status: Captura abortada. Dados descartados.");
        break;

    case Fase::SALVANDO:
        // Se terminou de ler mas desistiu de salvar, limpa tudo e volta ao início
        mEspelhoCabo.clear();
        mFase = Fase::CONECTANDO;
        mLbStatus->setText("Status: Dados descartados. Pronto para nova leitura.");
        break;
    }

    // Como voltamos para o estado inicial nas opções acima, repovoamos as portas
    Povoar();

    // Atualiza cores e habilitação dos botões
    AtualizarInterface();
}
