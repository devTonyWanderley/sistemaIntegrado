// C:\DESENV\CPP\PARSER\main.cpp

//  --CONSOLIDAR--
//bool Ler(const std::filesystem::path& fonte);                 ..  Carrega mCaderneta .. fonte é um tmp
//bool Salvar(const std::filesystem::path& destino);
//bool Carregar(const std::filesystem::path& fonte);
//void CalcularCaderneta();                                     ..  Carrega mPontos
//std::string lerTmp(const std::filesystem::path& fonte);
//std::string truncarTexto(std::string& texto);
//std::vector<std::string> fatiar(std::string& texto);

#include "GD5.hpp"
#include <iostream>

//  C:\DESENV\CPP\INSTANCIA

int main()
{
    GD5::Gd5 inst;
    inst.Ler("C:/DESENV/CPP/SERIAL-I/build/Desktop-Debug/debug/TMP/20260517091828.tmp");    //  Carrega mCaderneta
    std::cout << "mCaderneta: " << inst.mCaderneta.size() << " Leitura\'s de \"inst\"" << std::endl;
    inst.CalcularCaderneta();
    std::cout << "mPontos: " << inst.mPontos.size() << " PontoNormalizado\'s de \"inst\"" << std::endl;
    GD5::Gd5 outraInst;
    outraInst.CarregarCad("C:/DESENV/CPP/SERIAL-I/build/Desktop-Debug/debug/TMP/outro.cad");
    std::cout << "mCaderneta: " << outraInst.mCaderneta.size() << " Leitura\'s de \"outraInst\"" << std::endl;
    outraInst.CalcularCaderneta();
    std::cout << "mPontos: " << outraInst.mPontos.size() << " PontoNormalizado\'s de \"outraInst\"" << std::endl;
    //  --APRESENTAR mPontos--
    for(GD5::PontoNormalizado &p : outraInst.mPontos)
        std::cout << p.nome
                  << ' '
                  << p.atri
                  << ' '
                  << p.abci
                  << ' '
                  << p.orde
                  << ' '
                  << p.cota
                  << std::endl;
    return 0;
}
