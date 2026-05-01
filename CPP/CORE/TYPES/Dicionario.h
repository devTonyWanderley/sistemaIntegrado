//  Dicionario.h
#pragma once
#include <vector>
#include <string_view>
#include <algorithm>
#include <cstdint>

class Dicionario {
public:
    // Retorna o ID da string. Se não existir, adiciona.
    uint32_t obterOuAdicionar(std::string_view texto);

    // Recupera o texto a partir do ID (para a UI)
    std::string_view recuperar(uint32_t id) const;

    // Ordena o índice para permitir busca binária (O log n)
    void organizar();

    // Reserva memória para evitar realocações durante o carregamento (SINAPI)
    void prealocar(size_t tamanhoTexto, size_t qtdIndices);

private:
    // O "Blocão" de memória contínuo
    std::vector<char> m_pool;

    // Tabela de offsets (cada uint32_t aponta para o início de uma string no m_pool)
    // Este vetor será o alvo da ordenação.
    std::vector<uint32_t> m_indices;

    // Auxiliar para busca binária sem converter bytes em objetos pesados
    bool comparar(uint32_t id, std::string_view texto) const;
};
