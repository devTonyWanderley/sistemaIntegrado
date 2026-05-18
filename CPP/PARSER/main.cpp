// C:\DESENV\CPP\PARSER\main.cpp
#include <iostream>
#include <chrono>
#include "EquipoParser.hpp"

// Classe de simulação para testar a infraestrutura do subprojeto
class MockParser : public ParserEquipo {
public:
    [[nodiscard]] std::string_view idEquipo() const noexcept override {
        return "Equipamento Simulado de Teste";
    }

    uint64_t parseStream(std::string_view fBuffer, std::function<void(const RegistroDado&)> record_callback) override {
        uint64_t contador_registros = 0;
        
        // Simula a leitura rápida de um registro a partir do buffer usando C++20 string_view
        if (!fBuffer.empty()) {
            RegistroDado dado;
            dado.tipo = TipoDado::Leitura;
            dado.nome = fBuffer.substr(0, 5); // Simula extração sem cópia
            dado.xAz  = "08114130";
            dado.yVt  = "11947530";
            
            record_callback(dado);
            contador_registros++;
        }
        
        return contador_registros;
    }
};

int main() {
    std::cout << "[INFO] Inicializando subsistema de parsing (C++20 / Win32 Target)\n";

    // Criamos o ponteiro da interface apontando para a implementação
    ParserEquipo* parser = new MockParser();
    std::cout << "[INFO] Equipamento ativo: " << parser->idEquipo() << "\n";

    // Simulação de uma string em memória (como se fosse o arquivo .tmp mapeado via Win32)
    std::string_view buffer_arquivo_tmp = "PONTO_A_DADOS_BRUTOS_DA_ESTACAO_GD5";

    auto t_inicio = std::chrono::high_resolution_clock::now();

    // Execução do pipeline de stream de alta performance
    uint64_t total = parser->parseStream(buffer_arquivo_tmp, [](const RegistroDado& registro) {
        std::cout << "[CALLBACK] Processado Ponto: " << registro.nome 
                  << " | Hz: " << registro.xAz 
                  << " | Vt: " << registro.yVt << "\n";
    });

    auto t_fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> tempo_ms = t_fim - t_inicio;

    std::cout << "[FIM] Processamento concluido. Total de registros: " << total << "\n";
    std::cout << "[PERF] Tempo de execucao do parse: " << tempo_ms.count() << " ms\n";

    delete parser;
    return 0;
}
