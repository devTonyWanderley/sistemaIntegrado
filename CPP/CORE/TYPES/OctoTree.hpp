//  C:\Tony\Soft\cpp\integrado\sistemaIntegrado\CPP\CORE\TYPES\OctoTree.hpp
#pragma once
#include <cstdint>
#include <limits>
#include <string>
#include <vector>
/*
================================================================================
ARQUITETURA DO MOTOR ESPACIAL GENÉRICO (OCTREE)
================================================================================

1. FILOSOFIA DE DESIGN (DATA-ORIENTED DESIGN):
   - Separação total entre Geometria Pura (Locus) e Metadados Textuais (Quali).
   - Armazenamento em vetores globais contíguos na RAM para máxima localidade
     de cache (L1/L2) e alinhamento rígido de memória.
   - Indexação baseada em índices numéricos (uint32_t) eliminando ponteiros
     brutos, prevenindo memory leaks e facilitando a serialização em disco.

2. TRANSFORMAÇÃO E ANISOTROPIA CONTROLADA (ITEM 1 & 3):
   - Mapeamento não-uniforme: Cada eixo (X, Y, Z) é transladado pelo 'mOffSet'
     e esticado de forma independente pelo seu respectivo 'mEscala'.
   - Objetivo: Ocupar 100% da amplitude do tipo uint32_t (0 a ~4.29 bilhões),
     garantindo resolução sub-milimétrica homogênea (inclusive no eixo Z).
   - Unidade interna da árvore: "Quantum de Espaço Discreto" (Pixels 3D).

3. PROCESSO DE INGESTÃO DE DADOS DOS PARSERS (ITEM 2):
   - Fluxo em duas passagens obrigatórias:
     Passagem 1: Varredura linear em metros (double) para calcular extremos
                 absolutos (caixa delimitadora real) e povoar o dicionário.
     Passagem 2: Cálculo das constantes de escala/offset com margem de
                 segurança (padding) e colapso dos dados para uint32_t.

4. ESTRATÉGIA DE BUSCA POR RAIO (RADIUS SEARCH):
   - O espaço discreto interno é geometricamente distorcido (Anisotrópico).
   - Uma esfera de busca real (metros) se torna um ELIPSOIDE na Octree.
   - Solução em duas etapas para mitigar o custo computacional:
     Etapa A (Hierárquica): Transforma o raio em uma caixa retangular (AABB)
             discreta usando as escalas individuais. A árvore navega e faz
             o descarte (Pruning) de ramos usando apenas bits rápidos (< ou >).
     Etapa B (Euclidiana): Nos nós folhas, a validação exata
              é feita SEM RETORNAR PARA DOUBLE, utilizando aritmética inteira
              ponderada de 64 bits (uint64_t) para máxima velocidade de clock,
              eliminando conversões e mantendo a localidade de cache.
================================================================================
*/

namespace OT
{
struct Locus
{
    uint32_t abci = std::numeric_limits<uint32_t>::max();
    uint32_t orde = std::numeric_limits<uint32_t>::max();
    uint32_t cota = std::numeric_limits<uint32_t>::max();
};

struct alignas(8) Quali
{
    uint32_t Nome = std::numeric_limits<uint32_t>::max();
    uint32_t Atri = std::numeric_limits<uint32_t>::max();
};

struct alignas(8) Ponto
{
    uint32_t locus = std::numeric_limits<uint32_t>::max();
    uint32_t quali = std::numeric_limits<uint32_t>::max();
};

class OctoTree
{
private:
    std::vector<std::string> mDict;
    std::vector<Locus> mCoords;
    std::vector<Quali> mMDados;
    std::vector<Ponto> mPontos;
    double mOffSet[3] = {0.0, 0.0, 0.0};
    double mEscala[3] = {1.0, 1.0, 1.0};
public:
    OctoTree() = default;
};
}