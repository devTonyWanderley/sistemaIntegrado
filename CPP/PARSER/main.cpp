// C:\DESENV\CPP\PARSER\main.cpp
#include <iostream>
#include <chrono>
#include "EquipoParser.hpp"

#include <fstream>
#include <sstream>

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

int main()
{
    std::string leituras = lerLimpo();
    while(!leituras.empty() && leituras.find('_') != std::string::npos)
    {
        std::string tx = truncarTexto(leituras);
        std::cout << '\n' << tx.length() << '\t' ;
        std::vector<std::string> registro = fatiar(tx);
        for(auto& reg : registro) std::cout << reg << '\t';
    }
    return 0;
}
