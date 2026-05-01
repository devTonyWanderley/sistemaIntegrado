#include <iostream>
#include <fstream>
#include <chrono>
#include "SerialWin32.h"

// Variáveis globais para o teste (acessíveis pelo callback)
std::ofstream arquivoRaw;
auto ultimaAtividade = std::chrono::steady_clock::now();

// O "Manual Vivo": Esta função é o que o driver chamará para cada byte
void processadorDeCaptura(uint8_t byte) {
    // 1. Salva no disco (A camada de aplicação decide como salvar)
    if (arquivoRaw.is_open()) {
        arquivoRaw.put(static_cast<char>(byte));
    }

    // 2. Mostra na tela para conferência visual
    std::cout << (char)byte << std::flush;

    // 3. Atualiza o cronômetro de atividade
    ultimaAtividade = std::chrono::steady_clock::now();
}

int main() {
    SerialWin32 serial;
    SerialParams config;
    config.raw = SerialParams::build(9600, 0, 0, 8); // 9600, N, 8, 1

    // Prepara o arquivo de saída
    arquivoRaw.open("validacao_estacao.raw", std::ios::binary);

    std::cout << "--- TESTE DE VALIDACAO SERIAL ---" << std::endl;
    std::cout << "Aguardando transmissao... (Timeout de 3s para encerrar)" << std::endl;

    if (serial.abrir("COM3", config, processadorDeCaptura)) {

        // Loop de monitoramento de fim de transmissão
        while (true) {
            auto agora = std::chrono::steady_clock::now();
            auto tempoInativo = std::chrono::duration_cast<std::chrono::seconds>(agora - ultimaAtividade).count();

            // Se passaram 3 segundos desde o último byte, encerramos
            if (tempoInativo >= 3) {
                break;
            }
            Sleep(100); // Não consome CPU enquanto monitora
        }

        serial.fechar();
        arquivoRaw.close();
        std::cout << "\n--- TRANSMISSAO ENCERRADA POR INATIVIDADE ---" << std::endl;
    } else {
        std::cerr << "Erro ao abrir a porta COM3." << std::endl;
    }

    return 0;
}
