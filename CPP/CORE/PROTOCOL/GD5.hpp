//	C:\Tony\Soft\cpp\integrado\sistemaIntegrado\CPP\CORE\PROTOCOL\GD5.hpp
#pragma once
#include <cstdint>
#include <limits>
#include <string>
#include <vector>
#include <filesystem>
#include "../CORE/TYPES/OctoTree.hpp"

namespace GD5
{

#pragma pack(push, 1)
struct Leitura
{
    uint32_t aHor = std::numeric_limits<std::uint32_t>::max();
    uint32_t aVer = std::numeric_limits<std::uint32_t>::max();
    uint32_t dist = std::numeric_limits<std::uint32_t>::max();
    uint16_t altu = std::numeric_limits<std::uint16_t>::max();
    char nome[13] = {0};
    char atri[13] = {0};
};

struct PontoNormalizado
{
    uint32_t abci = std::numeric_limits<std::uint32_t>::max();
    uint32_t orde = std::numeric_limits<std::uint32_t>::max();
    uint32_t cota = std::numeric_limits<std::uint32_t>::max();
    char nome[13] = {0};
    char atri[13] = {0};
};
#pragma pack(pop)

struct PontoCalculado
{
    double x = std::numeric_limits<double>::max();
    double y = std::numeric_limits<double>::max();
    double z = std::numeric_limits<double>::max();
    char id[13];
    char cod[13];
};

class Gd5
{
public:
    Gd5() = default;
    bool Ler(const std::filesystem::path& fonte);
    bool Salvar(const std::filesystem::path& destino);
    bool CarregarCad(const std::filesystem::path& fonte);
    void CalcularCaderneta();

    std::vector<Leitura> mCaderneta;
    std::vector<PontoNormalizado> mPontos;  //  vai virar private
    std::vector<PontoCalculado> mPCal;
    // substituir os dois últimos por uma instância da classe OctoTree
private:
    std::string lerTmp(const std::filesystem::path& fonte);
    std::string truncarTexto(std::string& texto);
    std::vector<std::string> fatiar(std::string& texto);
};
}