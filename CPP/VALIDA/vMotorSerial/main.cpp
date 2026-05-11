#include <iostream>
#include <vector>
#include <span>
#include <string>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <filesystem> // Adicionado para verificar diretório
#include "motorSerial.hpp"
#include "configSerial.hpp"

namespace fs = std::filesystem;

// Controle de estatísticas global para o callback
std::atomic<size_t> g_totalBytes{0};
std::atomic<int> g_pacotes{0};

void callbackTerminal(std::span<const uint8_t> dados) {
    if (dados.empty()) return;
    g_totalBytes += dados.size();
    g_pacotes++;

    for (uint8_t b : dados) {
        if (b == '\r') continue;
        if (b == '\n') std::cout << "\n[DATA] ";
        else if (std::isprint(b)) std::cout << static_cast<char>(b);
        else std::cout << ".";
    }
    std::cout << std::flush;
}

void testarTransferenciaReal() {
    // 1. Garantir que o diretório de TMP existe (Evita falha no ofstream)
    std::string pathLog = "C:/DESENV/CPP/INSTANCIA/TMP/";
    try {
        if (!fs::exists(pathLog)) {
            fs::create_directories(pathLog);
            std::cout << "[SISTEMA] Diretorio criado: " << pathLog << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERRO] Falha ao gerenciar pastas: " << e.what() << std::endl;
        return;
    }

    MotorSerial motor;
    auto portas = MotorSerial::ListarPortas();

    if (portas.empty()) {
        std::cout << "\n[ERRO] Nenhuma porta COM detectada." << std::endl;
        return;
    }

    uint32_t baud = 9600;
    CfgSerial cfg;
    cfg.raw = CfgSerial::build(baud, 0, 1, 8);

    if (motor.Abrir(portas[0], cfg, callbackTerminal)) {
        // A classe MotorSerial já gerou o nome do arquivo internamente no Abrir()
        std::cout << "\n=====================================================" << std::endl;
        std::cout << "TERMINAL DE VALIDACAO - MODO GRAVACAO ATIVA" << std::endl;
        std::cout << "Porta: " << portas[0] << " | Baud: " << baud << std::endl;
        std::cout << "Status: Aguardando dados para iniciar gravacao..." << std::endl;
        std::cout << "=====================================================\n" << std::endl;

        size_t lastCount = 0;
        int inatividadeSegundos = 0;

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (g_totalBytes > lastCount) {
                if (lastCount == 0) {
                    std::cout << "\n[INFO] Dados detectados! Verifique o arquivo .tmp na pasta." << std::endl;
                }
                lastCount = g_totalBytes;
                inatividadeSegundos = 0;
            } else if (g_totalBytes > 0) {
                inatividadeSegundos++;
            }

            if (g_totalBytes > 0 && inatividadeSegundos >= 5) {
                std::cout << "\n\n[STATUS] Fim de transmissao (Inatividade)." << std::endl;
                break;
            }

            if (g_totalBytes == 0 && inatividadeSegundos >= 30) {
                std::cout << "\n[STATUS] Timeout: Sem dados da Estacao." << std::endl;
                break;
            }
        }

        motor.Fechar(); // Aqui o arquivo .tmp é devidamente fechado
        std::cout << "\n--- RESUMO DA OPERACAO ---" << std::endl;
        std::cout << "Arquivo gerado em: " << pathLog << std::endl;
        std::cout << "Total gravado: " << g_totalBytes << " bytes em " << g_pacotes << " pacotes." << std::endl;
    } else {
        std::cout << "[ERRO] Nao foi possivel abrir a porta " << portas[0] << std::endl;
    }
}

int main() {
    try {
        testarTransferenciaReal();
    } catch (const std::exception& e) {
        std::cerr << "Excecao: " << e.what() << std::endl;
    }
    std::cout << "\nPressione Enter para sair..." << std::endl;
    std::cin.get();
    return 0;
}
