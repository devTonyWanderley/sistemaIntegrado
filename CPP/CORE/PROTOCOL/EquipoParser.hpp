//	C:\DESENV\CPP\CORE\PROTOCOL\EquipoParser.hpp
/* ============================================================================
 * ESPECIFICAÇÃO DO PROTOCOLO DE DADOS TOPOGRÁFICOS (FIXED-WIDTH FORMAT)
 * ============================================================================
 *
 * ESTRUTURA GERAL DA LINHA:
 * - Cada campo possui o formato: [BANDEIRA][CÓDIGO][VALOR]
 * - Bandeira de campo: '@'
 * - Código: 2 dígitos numéricos.
 * - O campo '@10' (Nome) é OBRIGATÓRIO e deve ser SEMPRE o primeiro da linha.
 * - O aparecimento de um novo '@10' indica o fim da linha/registro anterior.
 * - Dispensa o uso de delimitadores de fim de linha como ';' ou '\n'.
 *
 * ----------------------------------------------------------------------------
 * TABELA DE CÓDIGOS, TAMANHOS E RESOLUÇÕES
 * ----------------------------------------------------------------------------
 * COD | DESCRIÇÃO           | TAM | FORMATO / RESOLUÇÃO / OBSERVAÇÕES
 * ----+---------------------+-----+-------------------------------------------
 * 10  | Nome do Ponto       | 12  | Texto. Alinhado à esquerda. Espaços pós.
 * 11  | Atributo / Desc     | 12  | Texto. Alinhado à esquerda. Espaços pós.
 * 20  | Altura Instrumento  |  6  | Unidade: 0.1 mm. Zeros pré. Amplitude: 9.99m (Ex: 016480 = 1.648m)
 * 21  | Altura Prisma/Alvo  |  6  | Unidade: 0.1 mm. Zeros pré. Amplitude: 9.99m (Ex: 015000 = 1.500m)
 * 30  | Ângulo Horizontal   |  7  | Formato: GGGMMSS. Zeros pré se GGG < 100.
 * 31  | Ângulo Vertical     |  7  | Formato: GGGMMSS. Zeros pré se GGG < 100.
 * 40  | Distância Inclinada |  8  | Unidade: 0.1 mm. Zeros pré. Amplitude: 9.99km (Ex: 00875815 = 87.5815m)
 * 41  | Dist. Horizontal    |  8  | Unidade: 0.1 mm. Zeros pré. Amplitude: 9.99km
 * 50  | Abcissa (Eeste / X) | 11  | Unidade: 0.1 mm. Sinal obrigatório (+/-) + 10 dígitos.
 *     |                     |     | Amplitude: ±999.999,9999 metros (Ideal para UTM).
 * 51  | Ordenada (Norte / Y)| 12  | Unidade: 0.1 mm. Sinal obrigatório (+/-) + 11 dígitos.
 *     |                     |     | Amplitude: ±9.999.999,9999 metros (Ideal para UTM).
 * 52  | Cota (Altitude / Z) |  9  | Unidade: 0.1 mm. Sinal obrigatório (+/-) + 8 dígitos.
 *     |                     |     | Amplitude: ±99.999,9999 metros.
 * ----------------------------------------------------------------------------
 *
 * EXPLICAÇÃO DAS MUDANÇAS E RESOLUÇÃO UTM:
 * 1. Coordenada Norte (51): Precisa de 12 caracteres no valor. O maior valor UTM
 *    no hemisfério sul é próximo a 10.000.000 metros. Em décimos de milímetro,
 *    isso exige 11 dígitos numéricos + 1 dígito para o sinal (+ ou -).
 *    Exemplo: -100000000000 equivale a -10.000.000,0000 metros.
 * 2. Coordenada Este (50): O valor UTM varia de ~160.000 a ~830.000 metros.
 *    10 dígitos numéricos + 1 sinal cobrem até 999.999 metros com folga.
 *
 * EXEMPLOS DE REGISTROS FORMATADOS:
 *
 * Estacionamento:
 * @10101         @11AUX         @20016480
 *
 * Visada (Ângulo Vertical corrigido para 7 dígitos '0895725' -> 089°57'25"):
 * @10102         @11CERCA       @300431205@310895725@4000875815@21015000
 *
 * Ponto UTM (Com o sinal integrado ao valor das coordenadas 50, 51 e 52):
 * @10M58         @11MARCO       @50+03024551545@51+100725414228@52+000100471
 *
 * Registro com Nome Vazio (Regra de início de registro mantida):
 * @10            @11OMITIDO     @50-01524331000@51+08221455000@52+000054120
 * ============================================================================

================================================================================
ESPECIFICAÇÃO DO FORMATO DE DADOS TOPOGRÁFICOS (ENGENHARIA REVERSA)
================================================================================
Características Gerais:
- Fluxo contínuo de caracteres (Stream) sem quebra de linha lógica obrigatória.
- Blocos de dados de comprimento fixo delimitados por caracteres de controle.
- Valores numéricos preenchidos com '0' à esquerda ou '*' para manter a largura.

--------------------------------------------------------------------------------
1. BLOCO DE CONFIGURAÇÃO DA ESTAÇÃO
Exemplo: _'0_(P_)1.676_
Sintaxe: _'[ID]_( [ATRIBUTO] _) [ALTURA_INSTRUMENTO] _

Estrutura dos Campos:
- _'       : Marcador de início do registro de estação.
- [ID]     : Identificador numérico da estação (Comprimento variável até '_').
- _(       : Delimitador de abertura do atributo.
- [ATRIB]  : Código/Atributo da estação (Ex: P), seguido por '_)'.
- [H_INST] : Altura do instrumento (Ex: 1.676 metros).
- _        : Caractere terminador do bloco de estação.

--------------------------------------------------------------------------------
2. BLOCO DE VISADA SEM DISTÂNCIA (ÂNGLOS PUROS)
Exemplo: _+NORTE_ <0955755+0000000+****d086_* _,1.600_
Sintaxe: _+[ID]_ <[V][H]+[LIXO]d[STATUS]_* _,[H_PRISMA]_

Estrutura dos Campos:
- _+       : Marcador de início de ponto visado.
- [ID]     : Nome/Alvo da visada (Ex: NORTE, 1). Terminador: '_'.
- <        : Indicador de bloco de medição angular (Ângulos puros).
- [V]      : Ângulo Vertical. Fixo em 7 dígitos (GGGMMSS). Ex: 0955755 = 095º57'55".
- +        : Separador de ângulos.
- [H]      : Ângulo Horizontal. Fixo em 7 dígitos (GGGMMSS). Ex: 0000000 = 000º00'00".
- +        : Separador.
- [LIXO]   : Preenchimento de tamanho fixo com '*' para dados não calculados.
- d[STATUS]: Código de erro/status interno do sensor (Ex: d086, d090).
- _* _,    : Sequência complexa de escape/transição de bloco.
- [H_PRI]  : Altura do prisma (Ex: 1.600 metros). Terminador: '_'.

--------------------------------------------------------------------------------
3. BLOCO DE LEITURA COMPLETA (DISTÂNCIA + ÂNGULOS)
Exemplo: _+1_ ?+00023023m0811410+11947530096\n d+00022754***+25-30050_*P_,1.600_
Sintaxe: _+[ID]_ ?+[DIST]m[V]+[H][SUFIXO]_*[ATRIBUTO]_,[H_PRISMA]_

Estrutura dos Campos:
- _+       : Marcador de início do ponto.
- [ID]     : Nome do ponto ordenado (Ex: 1, 2, 3). Terminador: '_'.
- ?+       : Indicador de início de bloco com distância inclinada.
- [DIST]   : Distância Inclinada em milímetros. Fixo em 8 dígitos, preenchido
             com zeros (Ex: 00023023 = 23.023 metros).
- m        : Sufixo 'metros'. Finaliza distância e inicia Ângulo Vertical.
- [V]      : Ângulo Vertical. Fixo em 7 dígitos (GGGMMSS). Ex: 0811410 = 081º14'10".
- +        : Separador de ângulos.
- [H]      : Ângulo Horizontal. Fixo em 7 dígitos (GGGMMSS). Ex: 1194753 = 119º47'53".
- [SUFIXO] : Bloco de telemetria interna calculada pelo aparelho contendo:
             - Dados de correção atmosférica/prisma (Ex: +25-30050).
             - Alinhamento de quebra de linha físico arbitrário (\n d+...).
- _*       : Sequência de escape para metadados do ponto.
- [ATRIB]  : Novo atributo caso alterado (Ex: P, M3, CERCA, PORTAO). Terminador: '_'.
- ,        : Separador de altura.
- [H_PRI]  : Altura do prisma associada ao ponto (Ex: 1.600). Terminador: '_'.
================================================================================
 */


#pragma once
#include <string>
#include <string_view>
#include <span>
#include <cstdint>
#include <functional>

struct RegistroDado
{
    std::string_view dados[6];

    constexpr RegistroDado(): dados{}{}

    constexpr RegistroDado(std::string_view id, std::string_view atr, std::string_view hi): dados{id, atr, hi, "", "", ""}{}

    constexpr RegistroDado(std::string_view id, std::string_view atr, std::string_view x, std::string_view y, std::string_view z):
        dados{id, atr, x, y, z, ""}{}

    constexpr RegistroDado(std::string_view id, std::string_view atr, std::string_view ah, std::string_view av, std::string_view di,
        std::string_view hs): dados{id, atr, ah, av, di, hs}{}

    constexpr std::string_view nome() const noexcept {return dados[0];}
    constexpr std::string_view atri() const noexcept {return dados[1];}
    constexpr std::string_view hIns() const noexcept {return (dados[3].empty())? dados[2] : "";}
    constexpr std::string_view aHor() const noexcept {return (dados[5].empty())? "" : dados[2];}
    constexpr std::string_view abci() const noexcept {return (dados[5].empty())? dados[2] : "";}
    constexpr std::string_view aVer() const noexcept {return (dados[5].empty())? "" : dados[3];}
    constexpr std::string_view orde() const noexcept {return (dados[5].empty())? dados[3] : "";}
    constexpr std::string_view dist() const noexcept {return (dados[5].empty())? "" : dados[4];}
    constexpr std::string_view cota() const noexcept {return (dados[5].empty())? dados[4] : "";}
    constexpr std::string_view hPri() const noexcept {return dados[5];}
};

class ParserEquipo  //  classe base
{
public:
    using cbReg = std::function<void(const RegistroDado&)>;
    virtual ~ParserEquipo() = default;
    [[nodiscard]] virtual std::string_view idEquipo() const noexcept = 0;
    virtual uint64_t parse(std::string_view stream, cbReg rcb) = 0;
};

class ParserGD5 : public ParserEquipo
{
public:
    constexpr ParserGD5() noexcept = default;
    ~ParserGD5() override = default;

    [[nodiscard]] std::string_view idEquipo() const noexcept override {return "GD5";}
    uint64_t parse(std::string_view stream, cbReg rcb) override
    {
        if (stream.empty() || !rcb) return 0;
        std::string bufferLimpo = higienizarStream(stream);
        //-----------------------------------------------//
        //-----------------------------------------------//
        std::string_view sv(bufferLimpo);
        uint64_t registrosProcessados = 0;
        size_t indice = 0;
        while (indice < sv.length())
        {
            indice = sv.find('_', indice);
            if (indice == std::string_view::npos) break;
            if (indice + 1 >= sv.length()) break;
            char marcador = sv[indice + 1];
            size_t proximoIndice = indice + 1;
            if (marcador == '\'') proximoIndice = processarBlocoEstacao(sv, indice, rcb, registrosProcessados);
            else if (marcador == '+') proximoIndice = processarBlocoPonto(sv, indice, rcb, registrosProcessados);
            else proximoIndice = indice + 1;
            indice = (proximoIndice > indice) ? proximoIndice : indice + 1;
        }
        return registrosProcessados;
    }
private:
    std::string higienizarStream(std::string_view original)
    {
        std::string r = "";
        r.reserve(original.size() * 128 / 136);
        size_t pos = 1;
        while(pos < original.size())
        {
            size_t n = (pos + 128 < original.size())? 128 : original.size() - pos;
            r += original.substr(pos, n);
            pos += 136;
        }
        std::string_view sv(r);
        std::string res = "";
        res.reserve(sv.length());
        for(auto i = 0; i < sv.length(); i++)
            if(sv[i] != ' ') res.push_back(sv[i]);
        return res;
    }
    size_t processarBlocoEstacao(std::string_view stream, size_t pos, const cbReg& rcb, uint64_t& contador)
    {
        size_t fimBloco = stream.find('_', pos + 2);
        if (fimBloco == std::string_view::npos) return stream.length();
        std::string_view bloco = stream.substr(pos + 2, fimBloco - (pos + 2));
        size_t abertAtrib = bloco.find("_(");
        size_t fechAtrib = bloco.find("_)");
        if (abertAtrib != std::string_view::npos && fechAtrib != std::string_view::npos)
        {
            std::string_view id = bloco.substr(0, abertAtrib);
            std::string_view atrib = bloco.substr(abertAtrib + 2, fechAtrib - (abertAtrib + 2));
            std::string_view hInst = bloco.substr(fechAtrib + 2);
            rcb(RegistroDado(id, atrib, hInst));
            contador++;
        }
        return fimBloco;
    }

    size_t processarBlocoPonto(std::string_view stream, size_t pos, const cbReg& rcb, uint64_t& contador)
    {
        size_t fimId = stream.find('_', pos + 2);
        if (fimId == std::string_view::npos) return stream.length();
        std::string_view id = stream.substr(pos + 2, fimId - (pos + 2));
        size_t proximo = fimId + 1;
        if (proximo >= stream.length()) return stream.length();
        if (stream[proximo] == '<') return processarVisadaAngular(stream, proximo, id, rcb, contador);
        if (stream[proximo] == '?') return processarMedicaoCompleta(stream, proximo, id, rcb, contador);
        return proximo;
    }

    size_t processarVisadaAngular(std::string_view stream, size_t pos, std::string_view id, const cbReg& rcb, uint64_t& contador)
    {
        size_t fimAng = stream.find('>', pos);
        if (fimAng == std::string_view::npos) return stream.length();
        std::string_view blocoAng = stream.substr(pos + 1, fimAng - (pos + 1));
        size_t primMais = blocoAng.find('+');
        size_t segMais = (primMais != std::string_view::npos) ? blocoAng.find('+', primMais + 1) : std::string_view::npos;
        if (primMais != std::string_view::npos && segMais != std::string_view::npos)
        {
            std::string_view av = blocoAng.substr(0, primMais);
            std::string_view ah = blocoAng.substr(primMais + 1, segMais - (primMais + 1));
            std::string_view hPri = buscarAlturaPrisma(stream, fimAng);
            rcb(RegistroDado(id, "", ah, av, "", hPri));
            contador++;
        }
        return fimAng;
    }

    size_t processarMedicaoCompleta(std::string_view stream, size_t pos, std::string_view id, const cbReg& rcb, uint64_t& contador)
    {
        size_t idxM = stream.find('m', pos);
        size_t idxMais = stream.find('+', pos);
        if (idxM == std::string_view::npos || idxMais == std::string_view::npos || idxMais > idxM) return pos + 1;
        std::string_view dist = stream.substr(idxMais + 1, idxM - (idxMais + 1));
        if (idxM + 1 + 7 <= stream.length() && idxM + 9 + 7 <= stream.length())
        {
            std::string_view av = stream.substr(idxM + 1, 7);
            std::string_view ah = stream.substr(idxM + 9, 7);
            std::string_view atrib = buscarAtributoOpcional(stream, idxM);
            std::string_view hPri = buscarAlturaPrisma(stream, idxM);
            rcb(RegistroDado(id, atrib, ah, av, dist, hPri));
            contador++;
        }
        size_t proximoBloco = stream.find('_', idxM);
        return (proximoBloco != std::string_view::npos) ? proximoBloco : stream.length();
    }

    std::string_view buscarAlturaPrisma(std::string_view stream, size_t posicaoBusca) const noexcept
    {
        size_t idxVirgula = stream.find(',', posicaoBusca);
        if (idxVirgula != std::string_view::npos)
        {
            size_t fimPrisma = stream.find('_', idxVirgula);
            if (fimPrisma != std::string_view::npos) return stream.substr(idxVirgula + 1, fimPrisma - (idxVirgula + 1));
        }
        return "";
    }

    std::string_view buscarAtributoOpcional(std::string_view stream, size_t posicaoBusca) const noexcept
    {
        size_t idxAtrib = stream.find("_*", posicaoBusca);
        if (idxAtrib != std::string_view::npos)
        {
            size_t fimAtrib = stream.find('_', idxAtrib + 2);
            if (fimAtrib != std::string_view::npos) return stream.substr(idxAtrib + 2, fimAtrib - (idxAtrib + 2));
        }
        return "";
    }
};