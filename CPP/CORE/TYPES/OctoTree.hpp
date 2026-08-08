//  C:\Tony\Soft\cpp\integrado\sistemaIntegrado\CPP\CORE\TYPES\OctoTree.hpp |   Compilador: GCC (MinGW), Versao: 13.1
#pragma once
#include "TiposElementares.hpp"
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

Enriquecimento com Morton e uso do uint128_t:
================================================================================
DIRETRIZES DE ARQUITETURA E CONCLUSÕES DO DESIGN ESPACIAL (MORTON HÍBRIDO)
================================================================================

1. RESOLUÇÃO E CHAVE GERAL (uint128_t):
   - Chave Morton unificada de 128 bits (42 bits/eixo) para indexação global.
   - Unidade espacial discreta fixada em Décimo de Milímetro (0.1 mm).
   - Amplitude linear máxima de ~439.804 km (Cobre o planeta em uma única raiz).
   - Elimina a necessidade de malhas ou vetores de raízes coirmãs no runtime.

2. LOGALIDADE DE CACHE E PESO EM MEMÓRIA:
   - Ordenação estática por Código Morton ao fim da Passagem 2 via std::sort.
   - O mDict ordenado garante buscas de metadados estáticos em O(log N).
   - Chaves de 128 bits custam 16 bytes. Comparação de chaves (<, >, ==) gera
     apenas 2 instruções de máquina na CPU 64-bits. Impacto no clock é nulo.
   - O verdadeiro gargalo evitado: Cache Misses eliminados pela linearidade.

3. DESEMPACOTAMENTO E BUSCA POR RAIO (RADIUS SEARCH):
   - Etapa A (Filtro BBox): Delimitação rápida de intervalo linear de memória
     operando diretamente nos limites [M_min, M_max] em uint128_t.
   - Etapa B (Validação na Folha): O código Morton de 128 bits dos candidatos é
     desempacotado via Bit-Deinterleaving (máscaras e shifts) para X, Y, Z.
   - Execução Numérica: Coordenadas isoladas e cálculos de elipsoide inteira
     ponderada rodam estritamente em uint64_t nativo (1 ciclo de clock).
   - Evita totalmente multiplicações em 128 bits e cálculos em double.
================================================================================
*/

namespace OT
{
class OctoTree
{
private:
    std::vector<std::string> mDict; //  dicionário de strings
    std::vector<unsigned __int128> mCoords;    //  banco de coordenadas empacotadas (morton) e ordenadas
    std::vector<Geom::iPonto> mIPontos;
    double mOffSet[3] = {0.0, 0.0, 0.0};
    double mEscala[3] = {1.0, 1.0, 1.0};

    unsigned __int128 espalhaBits3d(std::uint64_t n);
    unsigned __int128 geraMorton3d(std::uint64_t x, std::uint64_t y, std::uint64_t z);

    template<typename T>
    std::uint32_t buscaBinaria(const std::vector<T>& v, const T& e) const
    {
        std::uint32_t esq = 0, dir = v.size() - 1;
        while(esq <= dir)
        {
            std::uint32_t meio = esq + ((dir - esq) / 2);
            if(v.at(meio) == e) return meio;
            if(v.at(meio) > e) esq = meio;
            else dir = meio;
        }
        return std::numeric_limits<std::uint32_t>::max();
    }
    std::uint64_t compctaBits(unsigned __int128 m);
    double lerCoordi(int i, unsigned __int128 m);
public:
    OctoTree() = default;

    void carregaOT(std::vector<Geom::PontoMetrico>& pontos);
};

}