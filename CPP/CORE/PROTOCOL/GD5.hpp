//	GD5.hpp
#pragma once
#include <cstdint>
#include <limits>
#include <string>
#include <vector>
#include <filesystem>

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
#pragma pack(pop)

class Gd5
{
public:
    Gd5() = default;
    bool Ler(const std::filesystem::path& fonte);
    bool Salvar(const std::filesystem::path& destino);

    std::vector<Leitura> mCaderneta;
private:
    std::string lerTmp(const std::filesystem::path& fonte);
    std::string truncarTexto(std::string& texto);
    std::vector<std::string> fatiar(std::string& texto);
};
}