//  C:\Tony\Soft\cpp\integrado\sistemaIntegrado\CPP\CORE\TYPES\TiposElementares.hpp
#pragma once
#include <cstdint>
#include <limits>
#include <string>

namespace Geom
{

struct PontoMetrico //  O cliente cria uma coleção
{
    double x = std::numeric_limits<double>::max();
    double y = std::numeric_limits<double>::max();
    double z = std::numeric_limits<double>::max();
    std::string nome;
    std::string atri;
};   //  fim de "struct PontoMétrico"

struct LeituraGD5 //  O cliente cria uma coleção
{
    std::uint32_t aHor = std::numeric_limits<std::uint32_t>::max();
    std::uint32_t aVer = std::numeric_limits<std::uint32_t>::max();
    std::uint32_t dist = std::numeric_limits<std::uint32_t>::max();
    std::uint16_t altu = std::numeric_limits<std::uint16_t>::max();
    std::string nome;
    std::string atri;
};   //  fim de "struct LeituraGD5"

struct iPonto
{
    std::uint32_t iNome = std::numeric_limits<std::uint32_t>::max();
    std::uint32_t iAtri = std::numeric_limits<std::uint32_t>::max();
    std::uint32_t iMorton = std::numeric_limits<std::uint32_t>::max();
};

}   //  fim de "namespace Geom"