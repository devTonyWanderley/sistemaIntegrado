//  mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>

Q_DECLARE_METATYPE(ConfigSerial)

/*
void MainWindow::carregarPerfisPadrao() {
    // Lista de perfis baseada em equipamentos reais
    QList<PerfilEquipamento> perfis = {
        {"Leica TC 407 / TCR",    {9600,   8, Paridade::Nenhum, StopBits::Um}},
        {"Leica TS Series",       {115200, 8, Paridade::Nenhum, StopBits::Um}},
        {"Topcon (GTS/GPT)",      {9600,   8, Paridade::Nenhum, StopBits::Um}},
        {"Nikon DTM / Nivo",      {4800,   8, Paridade::Nenhum, StopBits::Um}},
        {"Sokkia SET",            {9600,   8, Paridade::Nenhum, StopBits::Um}},
        {"Geodetic GD5 / South",  {115200, 8, Paridade::Nenhum, StopBits::Um}},
        {"Trimble M3",            {9600,   8, Paridade::Nenhum, StopBits::Um}},
        {"Pentax",                {1200,   8, Paridade::Nenhum, StopBits::Um}}
    };

    ui->comboPerfis->clear();
    for (const auto& perfil : perfis) {
        ui->comboPerfis->addItem(QString::fromStdString(perfil.nome),
                                 QVariant::fromValue(perfil.config));
    }
}
*/

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Instanciar componentes
    m_configWidget = new ConfigSerialWidget(); // Widget flutuante de config
    m_timerLeitura = new QTimer(this);
    m_dadosBrutos.clear();

    // 2. Registrar o tipo para o ComboBox (essencial para o currentData)
    qRegisterMetaType<ConfigSerial>("ConfigSerial");
    // 3. Popular a lista de perfis de Estações Totais
    QList<PerfilEquipamento> perfis = {
        {"Leica TC 407 / TCR",    {9600,   8, Paridade::Nenhum, StopBits::Um}},
        {"Leica TS Series",       {115200, 8, Paridade::Nenhum, StopBits::Um}},
        {"Topcon (GTS/GPT)",      {9600,   8, Paridade::Nenhum, StopBits::Um}},
        {"Nikon DTM / Nivo",      {4800,   8, Paridade::Nenhum, StopBits::Um}},
        {"Geodetic GD5 / South",  {115200, 8, Paridade::Nenhum, StopBits::Um}},
        {"Sokkia SET",            {9600,   8, Paridade::Nenhum, StopBits::Um}},
        {"Trimble M3",            {9600,   8, Paridade::Nenhum, StopBits::Um}},
        {"Pentax",                {1200,   8, Paridade::Nenhum, StopBits::Um}}
    };

    for (const auto& p : perfis) {
        ui->comboPerfis->addItem(QString::fromStdString(p.nome), QVariant::fromValue(p.config));
    }
    // 4. Configurar o loop de leitura (Polling)
    connect(m_timerLeitura, &QTimer::timeout, this, [this]() {
        char buffer[1024]; // Buffer temporário
        int32_t lidos = m_motor.lerParaBuffer(buffer, sizeof(buffer) - 1);

        if (lidos > 0) {
            buffer[lidos] = '\0'; // Garante terminação de string C
            m_dadosBrutos.append(buffer, lidos);
            // Converte para Latin1 (ASCII puro) e insere no log
            ui->txtLog->insertPlainText(QString::fromLatin1(buffer));
            // Auto-scroll
            ui->txtLog->ensureCursorVisible();
        }
    });
}

void MainWindow::on_btnConfigManual_clicked()
{
    // 1. Abre a janela de configuração manual que você criou
    // Usamos setWindowModality para o usuário focar na config antes de ler
    m_configWidget->setWindowModality(Qt::ApplicationModal);
    m_configWidget->setWindowTitle("Ajuste Manual de Parâmetros");
    m_configWidget->show();

    // 2. Quando o usuário clica em Configuração Manual,
    // limpamos a seleção do combo de perfis para o sistema saber
    // que deve usar os dados do Widget no próximo "Conectar"
    ui->comboPerfis->setCurrentIndex(-1);

    // Opcional: Log visual para o usuário
    ui->txtLog->append("[SISTEMA] Modo de configuração manual ativado.");
}

void MainWindow::on_btnIniciarDownload_clicked()
{
    if (m_motor.estaConectado())
    {
        m_motor.desconectar();
        m_timerLeitura->stop();
        ui->txtLog->append("\n[LOG] Conexão encerrada.");
        return;
    }
    ConfigSerial config;
    if (ui->comboPerfis->currentIndex() != -1) config = ui->comboPerfis->currentData().value<ConfigSerial>();
    else config = m_configWidget->obterConfiguracao();
    int porta = m_configWidget->obterPortaSelecionada();

    if (m_motor.conectar(porta, config)) {
        QString nomeEquipamento = (ui->comboPerfis->currentIndex() != -1)
        ? ui->comboPerfis->currentText()
        : "Configuração Manual";

        ui->txtLog->append(">>> Lendo: " + nomeEquipamento);
        m_timerLeitura->start(50);
    }
    else ui->txtLog->append("[ERRO] Falha ao abrir porta.");
}

void MainWindow::on_btnSalvar_clicked()
{
    QString filtro = "Dados Topográficos (*.raw *.idx *.txt *.dat *.dxf);;Todos (*.*)";
    QString caminho = QFileDialog::getSaveFileName(this, "Salvar Dados ASCII", "", filtro);
    if (caminho.isEmpty()) return;
    QFile file(caminho);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(m_dadosBrutos);
        file.close();
        m_dadosBrutos.clear();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

