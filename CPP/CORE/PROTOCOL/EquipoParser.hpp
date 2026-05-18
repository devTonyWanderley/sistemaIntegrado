//	C:\DESENV\CPP\CORE\PROTOCOL\EquipoParser.hpp
#pragma once
#include <string_view>
#include <cstdint>
#include <functional>

enum class TipoDado : uint8_t {Estacao = 0, Leitura = 1, Coordenada = 2};

//  Estação: nome, atributo, altura do instrumento (3)
//  Visada: nome, atributo, ângulo horizontal, ângulo vertical, distância inclinada, altura do prisma (6)
//  Ponto: nome, atributo, abcissa, ordenada, cota (5)

//  |Estação    |Leitura    |Coordenada |
//  |Nome       |Nome       |Nome       |   ->  nome
//  |Atributo   |Atributo   |Atributo   |   ->  atri
//  |           |Azimute    |Abcissa    |   ->  xAzi
//  |           |Vertical   |Ordenada   |   ->  yVer
//  |           |Distância  |           |   ->  dist
//  |Altura hi  |Altura hs  |Cota       |   ->  zAlt

/*
 * formato final:
exemplos:
0 10+MARCO_A 11+POLIGONAL 83+1.575
1 10+101 11+CERCA 21+145.2530 22+89.1240 31+124.785 83+1.650
2 10+101 11+CERCA 21+742105.120 22+Plan_Y 83+542.150
descrição
[tipo] 10+[nome] 11+[atributo] 21+[azimute / abcissa] 22+[vertical / ordenada] 31+[distancia] 83+[hi / hs / cota]
*/

struct alignas(8) RegistroDado
{
    TipoDado tipo;
    std::string_view nome;
    std::string_view atri;
    std::string_view xAzi;
    std::string_view yVer;
    std::string_view dist;
    std::string_view zAlt;
};

class ParserEquipo
{
public:
    virtual ~ParserEquipo() = default;
    [[nodiscard]] virtual std::string_view idEquipo() const noexcept = 0;
    virtual uint64_t parseStream(std::string_view fBuffer, std::function<void(const RegistroDado&)> record_callback) = 0;
};

class ParserGD5 : public ParserEquipo
{
public:
    [[nodiscard]] std::string_view idEquipo() const noexcept override {
        return "Geodetic GD5 (.M21)";
    }

    uint64_t parseStream(std::string_view fBuffer, std::function<void(const RegistroDado&)> record_callback) override
    {
        uint64_t contador = 0;
        if (fBuffer.empty()) return 0;
        //  Processamento pra retornar M21
        //record_callback(dado_teste);
        contador++;
        return contador;
    }
};