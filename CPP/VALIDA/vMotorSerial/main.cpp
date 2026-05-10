#include <iostream>
#include <vector>
#include <span>
#include <string>
#include <chrono>
#include <atomic>
#include <iomanip>
#include "motorSerial.hpp"
#include "configSerial.hpp"

// Controle de estatísticas global para o callback
std::atomic<size_t> g_totalBytes{0};
std::atomic<int> g_pacotes{0};

// Callback estilo "Dump de Terminal"
void callbackTerminal(std::span<const uint8_t> dados) {
    if (dados.empty()) return;

    g_totalBytes += dados.size();
    g_pacotes++;

    // Mostra os dados na tela (sanitizados para não quebrar o console)
    for (uint8_t b : dados) {
        if (b == '\r') continue; // Ignora CR para evitar sobrescrita
        if (b == '\n') std::cout << "\n[DATA] ";
        else if (std::isprint(b)) std::cout << static_cast<char>(b);
        else std::cout << "."; // Representa caracteres de controle/binários
    }
    std::cout << std::flush;
}

void testarTransferenciaReal() {
    MotorSerial motor;
    auto portas = MotorSerial::ListarPortas();

    if (portas.empty()) {
        std::cout << "\n[ERRO] Nenhuma porta COM detectada. Conecte o cabo da Estacao." << std::endl;
        return;
    }

    // Configuração: Ajuste aqui conforme o menu da sua Estação Total
    uint32_t baud = 9600; // Altere para 19200, 38400 ou 115200 conforme necessário
    CfgSerial cfg;
    cfg.raw = CfgSerial::build(baud, 0, 1, 8);

    std::cout << "\n=====================================================" << std::endl;
    std::cout << "TERMINAL DE VALIDACAO - MODO TRANSFERENCIA" << std::endl;
    std::cout << "Porta: " << portas[0] << " | Baud: " << baud << std::endl;
    std::cout << "Diretorio de Log: C:/DESENV/CPP/INSTANCIA/TMP/" << std::endl;
    std::cout << "=====================================================\n" << std::endl;

    if (motor.Abrir(portas[0], cfg, callbackTerminal)) {
        std::cout << "[STATUS] Aguardando dados... Inicie o 'Send' na Estacao Total." << std::endl;
        std::cout << "[LOG] ";

        size_t lastCount = 0;
        int inatividadeSegundos = 0;

        // Loop de monitoramento de fluxo
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (g_totalBytes > lastCount) {
                // Se o volume cresceu, houve atividade
                lastCount = g_totalBytes;
                inatividadeSegundos = 0;
            } else if (g_totalBytes > 0) {
                // Se já recebeu algo mas parou, conta o timeout
                inatividadeSegundos++;
            }

            // Critério de encerramento: 5 segundos de silêncio após receber algo
            if (g_totalBytes > 0 && inatividadeSegundos >= 5) {
                std::cout << "\n\n[STATUS] Fim de transmissao detectado (Inatividade)." << std::endl;
                break;
            }

            // Opcional: Timeout de segurança se nunca receber nada (ex: 30s)
            if (g_totalBytes == 0 && inatividadeSegundos >= 30) {
                std::cout << "\n[STATUS] Timeout: Nao houve inicio de transmissao." << std::endl;
                break;
            }
        }

        std::cout << "\n--- RESUMO DA VALIDACAO ---" << std::endl;
        std::cout << "Total de Bytes: " << g_totalBytes << std::endl;
        std::cout << "Total de Pacotes: " << g_pacotes << std::endl;
        std::cout << "Media por Pacote: " << (g_pacotes > 0 ? g_totalBytes / g_pacotes : 0) << " bytes" << std::endl;

        motor.Fechar();
    } else {
        std::cout << "[ERRO] Nao foi possivel abrir a porta " << portas[0] << std::endl;
    }
}

int main() {
    try {
        testarTransferenciaReal();
    } catch (const std::exception& e) {
        std::cerr << "Excecao no Teste: " << e.what() << std::endl;
    }

    std::cout << "\nProcesso Finalizado. Pressione Enter para fechar..." << std::endl;
    std::cin.get();
    return 0;
}
