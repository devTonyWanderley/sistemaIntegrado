//  PainelConexao.cpp
#include "PainelConexao.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QFileDialog>
#include <fstream>

// Inicializa o ponteiro como nulo no topo do arquivo .cpp
PainelConexao* PainelConexao::s_instanciaAtiva = nullptr;

PainelConexao::PainelConexao(QWidget* parent) : QWidget(parent), m_estado(Estado::IDLE)
{
    s_instanciaAtiva = this; // O painel se registra como o receptor ativo
    m_buffer.reserve(1024 * 100);
    montarLayout();
}

PainelConexao::~PainelConexao() {
    m_motor.fechar();       // Para a thread do Windows primeiro
    s_instanciaAtiva = nullptr; // Depois "desliga" a ponte
}


// A função que a WinAPI chama milhões de vezes
void PainelConexao::receptorEstatico(uint8_t byte) {
    if(!s_instanciaAtiva) return;

    if (s_instanciaAtiva && s_instanciaAtiva->m_estado == Estado::CAPTURANDO) {
        // Trava o buffer para ninguém mais mexer enquanto inserimos
        std::lock_guard<std::mutex> trava(s_instanciaAtiva->m_mutexBuffer);

        s_instanciaAtiva->m_buffer.push_back(byte);
    }
}

void PainelConexao::salvarComSTL(const std::string& caminho) {
    std::ofstream arquivo(caminho, std::ios::binary);
    if (arquivo.is_open()) {
        arquivo.write(reinterpret_cast<const char*>(m_buffer.data()), m_buffer.size());
        arquivo.close();
        m_buffer.clear(); // Limpa para a próxima captura
    }
}

void PainelConexao::montarLayout() {
    auto layoutPrincipal = new QVBoxLayout(this);

    // --- GRUPO 1: HARDWARE ---
    auto grupoHardware = new QGroupBox("Configuração de Hardware", this);
    auto layoutHw = new QFormLayout(grupoHardware);

    m_comboEquipamento = new QComboBox(this);
    for(const auto& e : LISTA_EQUIPAMENTOS) m_comboEquipamento->addItem(e.nome);

    m_comboPorta = new QComboBox(this);
    auto portasDisponiveis = SerialWin32::listarPortas();
    if(portasDisponiveis.empty())
    {
        m_comboPorta->addItem("NENHUMA PORTA");
        m_btnConectar->setEnabled(false); // Desativa o botão se não há cabo
        m_status->setText("ERRO: Conecte o cabo serial!");
        m_status->setStyleSheet("color: red;");
    }
    else
    {
        for(const auto &nome : portasDisponiveis) m_comboPorta->addItem(QString::fromStdString(nome));
        m_btnConectar->setEnabled(true);
    }

    m_comboBaud = new QComboBox(this);
    m_comboBaud->addItems({"4800", "9600", "19200", "38400", "115200"});

    // Adicionando os que faltavam (Paridade e StopBits)
    auto comboParidade = new QComboBox(this);
    comboParidade->addItems({"Nenhuma (N)", "Ímpar (O)", "Par (E)"});

    auto comboStop = new QComboBox(this);
    comboStop->addItems({"1 bit", "1.5 bits", "2 bits"});

    layoutHw->addRow("Equipamento:", m_comboEquipamento);
    layoutHw->addRow("Porta COM:", m_comboPorta);
    layoutHw->addRow("Baud Rate:", m_comboBaud);
    layoutHw->addRow("Paridade:", comboParidade);
    layoutHw->addRow("Stop Bits:", comboStop);

    // --- BOTÃO E STATUS ---
    // Criamos um layout horizontal para os botões de ação
    auto layoutBotoes = new QHBoxLayout();
    m_btnConectar = new QPushButton("ABRIR COMUNICAÇÃO", this);
    m_btnConectar->setMinimumHeight(50);

    m_btnDescartar = new QPushButton("DESCARTAR", this);
    m_btnDescartar->setMinimumHeight(50);
    m_btnDescartar->setVisible(false); // Começa invisível
    m_btnDescartar->setStyleSheet("background-color: #ffcccc; color: #990000;"); // Vermelho suave

    layoutBotoes->addWidget(m_btnConectar, 3); // Ocupa 3 partes
    layoutBotoes->addWidget(m_btnDescartar, 1); // Ocupa 1 parte

    m_status = new QLabel("Status: Offline", this);

    // ADICIONAR AO LAYOUT PRINCIPAL
    layoutPrincipal->addWidget(grupoHardware);
    //layoutPrincipal->addWidget(m_btnConectar);
    layoutPrincipal->addLayout(layoutBotoes);
    layoutPrincipal->addWidget(m_status);


    // --- LÓGICA DE ATUALIZAÇÃO AUTOMÁTICA ---
    connect(m_comboEquipamento, &QComboBox::currentIndexChanged, [this](int index){
        if (index >= 0) {
            // 1. Criamos uma struct temporária para "descompactar" os bits
            SerialParams p;
            p.raw = LISTA_EQUIPAMENTOS[index].params; // Agora usamos .params

            // 2. Extraímos o Baud real usando o idioma WinAPI (winBaud)
            m_comboBaud->setCurrentText(QString::number(p.winBaud()));

            // 3. Atualizamos a extensão
            m_labelExtensao->setText(LISTA_EQUIPAMENTOS[index].extensao);
        }
    });


    connect(m_btnConectar, &QPushButton::clicked, this, &PainelConexao::aoClicarConectar);
    // Conectar o descarte
    connect(m_btnDescartar, &QPushButton::clicked, [this]() {
        if (m_estado == Estado::PRONTO_PARA_SALVAR) {
            std::lock_guard<std::mutex> trava(m_mutexBuffer);
            m_buffer.clear();
            m_estado = Estado::IDLE;
            m_btnConectar->setText("NOVA CAPTURA");
            m_btnDescartar->setVisible(false);
            m_status->setText("Dados descartados.");
            m_status->setStyleSheet("");
        }
    });
}



void PainelConexao::aoClicarConectar() {
    if (m_estado == Estado::IDLE) {
        m_btnDescartar->setVisible(false);

        // 1. Precisamos montar a struct aqui para a função abrir() enxergar!
        uint32_t baud = m_comboBaud->currentText().toUInt();

        SerialParams params;
        // Usamos o perfil selecionado para pegar paridade/stop/bits
        const auto& perfil = LISTA_EQUIPAMENTOS[m_comboEquipamento->currentIndex()];
        params.raw = perfil.params; // Já traz a configuração certa
        // Se o usuário mudou o Baud manualmente no combo, sobrescrevemos:
        params.raw = (params.raw & ~0xFFFFF) | (baud & 0xFFFFF);

        m_buffer.clear(); // Limpa o "balde" de bytes antes de começar

        // 2. Abre com o callback estático
        std::string porta = m_comboPorta->currentText().toStdString();

        m_buffer.clear();
        if (m_motor.abrir(m_comboPorta->currentText().toStdString().c_str(), params, &PainelConexao::receptorEstatico))
        {
            m_estado = Estado::CAPTURANDO;
            m_btnConectar->setText("FINALIZAR E SALVAR");
            m_status->setText("Capturando dados...");
            m_status->setStyleSheet("background-color: green; color: white;");
        }
    }
    // ESTADO 2: CAPTURANDO -> PARAR
    else if (m_estado == Estado::CAPTURANDO) {
        m_motor.fechar();
        m_estado = Estado::PRONTO_PARA_SALVAR; // Mudança aqui
        m_btnConectar->setText("GRAVAR NO DISCO");
        m_status->setText("Leitura concluída. Dados em memória.");
        m_status->setStyleSheet("background-color: orange; color: black;");
    }
    // ESTADO 3: SALVAMENTO (Loop de tentativa)
    else if (m_estado == Estado::PRONTO_PARA_SALVAR) {
        QString filtro = "Arquivo (*" + m_labelExtensao->text() + ")";
        QString caminho = QFileDialog::getSaveFileName(this, "Salvar", "", filtro);

        if (!caminho.isEmpty()) {
            this->salvarComSTL(caminho.toStdString());

            // SÓ AQUI resetamos para o início
            m_btnConectar->setText("NOVA CAPTURA");
            m_estado = Estado::IDLE;
            m_status->setText("Salvo com sucesso.");
            m_status->setStyleSheet("");
        } else {
            // Se cancelar, permanece no estado PRONTO_PARA_SALVAR
            m_status->setText("Aguardando definição de arquivo...");
        }
    }
}

