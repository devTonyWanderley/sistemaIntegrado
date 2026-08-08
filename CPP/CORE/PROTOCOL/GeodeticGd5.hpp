//  C:\Tony\Soft\cpp\integrado\sistemaIntegrado\CPP\CORE\PROTOCOL\GeodeticGd5.hpp, substituindo ..\GD5.hpp
#pragma once
#include "../CORE/TYPES/OctoTree.hpp"
#include <filesystem>
#include <vector>

namespace Gdtc
{
class Geodetic
{
private:
    std::vector<Geom::LeituraGD5> mCaderneta;
    //std::vector<Geom::PontoMétrico> mPontos;
    std::string lerTmp(const std::filesystem::path& fonte);
    std::vector<std::string> fatiar(std::string& texto);
    std::string truncarTexto(std::string& texto);
public:
    std::vector<Geom::PontoMetrico> mPontos;   //  vai pra private
    Geodetic() = default;
    bool Ler(const std::filesystem::path& fonte);
    void CalcularCaderneta();
};
}  //  namespace Gdtc