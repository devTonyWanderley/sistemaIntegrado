//  PainelConexao.h
#pragma once
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <vector>
#include <string>
#include <mutex>
#include "SerialWin32.h"
#include "Equipamentos.h"

class PainelConexao : public QWidget
{
    Q_OBJECT
public:
    explicit PainelConexao(QWidget* parent = nullptr);

    virtual ~PainelConexao();

    enum class Estado {IDLE, CAPTURANDO, PRONTO_PARA_SALVAR};

private slots:
    void aoClicarConectar();

private:

    // Mutex para proteger o m_buffer
    std::mutex m_mutexBuffer;

    // Ponteiro estático para a instância ativa (A Ponte)
    static PainelConexao* s_instanciaAtiva;

    // O Motor (Soberania WinAPI)
    SerialWin32 m_motor;

    // Componentes Visuais (A "casca" Qt)
    QComboBox* m_comboEquipamento;
    QComboBox*   m_comboPorta;
    QComboBox*   m_comboBaud;
    QLineEdit*   m_inputArquivo;   // <-- Faltava declarar
    QLabel*      m_labelExtensao;  // <-- Faltava declarar
    QPushButton* m_btnConectar;
    QPushButton* m_btnDescartar; // <-- Adicionar este
    QLabel*      m_status;

    // Gerenciamento de Estado e Dados
    Estado m_estado;
    std::vector<uint8_t> m_buffer; // Onde os bytes ficam até salvar

    void montarLayout();
    void salvarComSTL(const std::string& caminho); // Declarar o método
    // Função que o SerialWin32 vai chamar (C-Style)
    static void receptorEstatico(uint8_t byte);
};

