// C:\DESENV\CPP\PARSER\main.cpp
#include <iostream>
#include <chrono>
//#include "EquipoParser.hpp"

//#include <fstream>
//#include <sstream>

#include "GD5.hpp"

/*
std::string ler()
{
    std::ifstream arquivo("C:\\DESENV\\CPP\\SERIAL-I\\build\\Desktop-Debug\\debug\\TMP\\20260517091828.tmp", std::ios::binary);
    if(!arquivo.is_open()) return "";
    std::stringstream r;
    r << arquivo.rdbuf();
    return r.str();
}

std::string lerLimpo()
{
    std::string original = ler();
    if(original.empty()) return original;
    std::string_view origem(original);
    std::string limpo = "";
    limpo.reserve(original.size() * (128 / 136));
    size_t posi = 1;
    while(posi < origem.size())
    {
        size_t n = (posi + 128 < origem.size())? 128 : origem.size() - posi;
        limpo += origem.substr(posi, n);
        posi += 136;
    }
    return limpo;
}

std::vector<std::string> fatiar(std::string& texto)
{
    if(texto.empty()) return {};
    std::vector<std::string> r;
    size_t posi = 0, posf = texto.find('_');
    r.push_back(texto.substr(posi, posf - posi));
    if(texto.find("_(") == std::string::npos)
    {
        posi = texto.find("_*") + 2;
        posf = texto.find('_', posi);
        r.push_back(texto.substr(posi, posf - posi));
        posi = texto.find("_,") + 2;
        r.push_back(texto.substr(posi));
        if(texto.find("?+") == std::string::npos)
        {
            posi = texto.find('<') + 1;
            posf = texto.find('+', posi);
            r.push_back(texto.substr(posi, posf - posi));
            posi = posf + 1;
            posf = texto.find('+', posi);
            r.push_back(texto.substr(posi, posf - posi));
            return r;
        }
        posi = texto.find("?+") + 2;
        posf = texto.find('m', posi);
        r.push_back(texto.substr(posi, posf - posi));
        posi = posf + 1;
        posf = texto.find('+', posi);
        r.push_back(texto.substr(posi, posf - posi));
        posi = posf + 1;
        posf = texto.find("d+", posi);
        r.push_back(texto.substr(posi, posf - posi));
        return r;
    }
    posi = texto.find("_(") + 2;
    posf = texto.find('_', posi);
    r.push_back(texto.substr(posi, posf - posi));
    posi = posf + 2;
    r.push_back(texto.substr(posi));
    return r;
}

std::string truncarTexto(std::string& texto)
{
    size_t e = texto.find("_\'"), v = texto.find("_+"), f = texto.find_last_of('_'), posi, posf;
    std::string r;
    if(e != std::string::npos && v != std::string::npos)
    {
        posi = (e < v)? e + 2 : v + 2;
        e = texto.find("_\'", posi);
        v = texto.find("_+", posi);
        if(e != std::string::npos && v != std::string::npos) posf = (e < v)? e : v;
        else if(e != std::string::npos) posf = e;
        else posf = v;
        r = texto.substr(posi, posf - posi);
        texto = texto.substr(posf, texto.length());
        return r;
    }
    if(e != std::string::npos)
    {
        posi = e + 2;
        e = texto.find("_\'", posi);
        posf = (e != std::string::npos)? e : f;
        r = texto.substr(posi, posf - posi);
        texto = texto.substr(posf, texto.length());
        return r;
    }
    if(v != std::string::npos)
    {
        posi = v + 2;
        v = texto.find("_+", posi);
        posf = (v != std::string::npos)? v : f;
        r = texto.substr(posi, posf - posi);
        texto = texto.substr(posf, texto.length());
        return r;
    }
    r = texto = "";
    return r;
}

void usandoRegistroDado()
{
    std::string fonte = lerLimpo();
    std::vector<RegistroDado> registros;
    while(!fonte.empty())
    {
        std::string tx = truncarTexto(fonte);
        std::vector<std::string> ddos = fatiar(tx);
        if(!ddos.empty())
        {
            if(ddos.size() == 3)
            {
                RegistroDado re(ddos[0], ddos[1], ddos[2], "", "", "");
                registros.push_back(re);
            }
            else if(ddos.size() == 5)
            {
                RegistroDado re(ddos[0], ddos[1], ddos[4], ddos[2], ddos[3], "");
                registros.push_back(re);
            }
            else
            {
                RegistroDado re(ddos[0], ddos[1], ddos[5], ddos[2], ddos[4], ddos[3]);
                registros.push_back(re);
            }
        }
    }
    for(RegistroDado& rdd : registros)
    {
        std::cout << rdd.nome() << '\t' << rdd.atri() << '\t';
        if(rdd.hIns().empty())
        {
            std::cout << rdd.aHor() << '\t' << rdd.aVer() << '\t' << rdd.hPri();
            if(!rdd.dist().empty()) std::cout << '\t' << rdd.dist();
        }
        else std::cout << rdd.hIns();
        std::cout << '\n';
    }
}
*/

int main()
{
    /*
    std::string leituras = lerLimpo();
    while(!leituras.empty())
    {
        std::string tx = truncarTexto(leituras);
        std::cout << '\n';
        std::vector<std::string> registro = fatiar(tx);
        for(auto& reg : registro) std::cout << reg << '\t';
    }
    usandoRegistroDado();
    */
    GD5::Gd5 instancia;
    instancia.Ler("C:\\DESENV\\CPP\\SERIAL-I\\build\\Desktop-Debug\\debug\\TMP\\20260517091828.tmp");
    std::cout << instancia.mCaderneta.size() << std::endl;
    for(GD5::Leitura a : instancia.mCaderneta)
    {
        std::cout
            << a.nome << ' '
            << a.atri << ' '
            << a.altu;
        if(a.aHor != std::numeric_limits<uint32_t>::max())
            std::cout << ' ' << a.aVer << ' ' << a.aHor;
        if(a.dist != std::numeric_limits<uint32_t>::max())
            std::cout << ' ' << a.dist;
        std::cout << '\n';
    }
    instancia.Salvar("C:\\DESENV\\CPP\\SERIAL-I\\build\\Desktop-Debug\\debug\\TMP\\outro.cad");
    return 0;
}
