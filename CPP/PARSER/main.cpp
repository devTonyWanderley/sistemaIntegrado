// C:\DESENV\CPP\PARSER\main.cpp
#include <iostream>
#include "GeodeticGd5.hpp"

//  C:\DESENV\CPP\INSTANCIA C:/Tony/Soft/cpp/integrado/sistemaIntegrado/CPP/INSTANCIA/TMP/20260510162421.tmp

int main()
{
    Gdtc::Geodetic inst;
    inst.Ler("C:/Tony/Soft/cpp/integrado/sistemaIntegrado/CPP/INSTANCIA/TMP/20260510162421.tmp");    //  Carrega mCaderneta
    inst.CalcularCaderneta();
    /*
    for(Geom::PontoMetrico& p : inst.mPontos)
    {
        std::cout
            << p.nome << ' '
            << p.atri << ' '
            << p.x << ' '
            << p.y << ' '
            << p.z << std::endl;
    }
    */
    OT::OctoTree teste;
    std::cout << "\n\tCriou o teste" << std::endl;
    teste.carregaOT(inst.mPontos);

    // daqui pra baixo, trava ....

    std::cout << "\n\tCarregou o teste" << std::endl;
    return 0;
}
