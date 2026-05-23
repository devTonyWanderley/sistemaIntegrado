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

int main()
{
    std::string leituras = lerLimpo();
    std::cout
        << leituras.length()
        << '\n'
        << leituras.find("_\'", 0) + 2
        << '\n'
        << leituras.find("_+", 0)
        << '\n'
        << leituras.substr(2, 11)
        << std::endl;
    size_t pos = 0, posf;
    while(true)
    {
        if(leituras.find("_\'", pos) != std::string::npos && leituras.find("_+", pos) != std::string::npos)
        {
            pos = (leituras.find("_\'", pos) < leituras.find("_+", pos))? leituras.find("_\'", pos) : leituras.find("_+", pos);
            pos += 2;
            if(leituras.find("_\'", pos) != std::string::npos && leituras.find("_+", pos) != std::string::npos)
                posf = (leituras.find("_\'", pos) < leituras.find("_+", pos))? leituras.find("_\'", pos) : leituras.find("_+", pos);
            else if(leituras.find("_\'", pos) != std::string::npos) posf = leituras.find("_\'", pos);
            else if(leituras.find("_+", pos) != std::string::npos) posf = leituras.find("_+", pos);
            else posf = leituras.length() - 1;
        }
        else if(leituras.find("_\'", pos) != std::string::npos)
        {
            pos = leituras.find("_\'", pos) + 2;
            if(leituras.find("_\'", pos) != std::string::npos) posf = leituras.find("_\'", pos);
            else posf = leituras.length() - 1;
        }
        else if(leituras.find("_+", pos) != std::string::npos)
        {
            pos = leituras.find("_+", pos) + 2;
            if(leituras.find("_+", pos) != std::string::npos) posf = leituras.find("_+", pos);
            else posf = leituras.length() - 1;
        }
        else break;
        std::string linha = leituras.substr(pos, posf - pos);
        if(linha.find("_(") != std::string::npos)
        {
            size_t i = 0, j = linha.find('_');
            std::cout << linha.substr(i, j - i) << '\t';
            i = linha.find('(') + 1;
            j = linha.find('_', i);
            std::cout << linha.substr(i, j - i) << '\t';
            i = linha.find(')') + 1;
            std::cout << linha.substr(i) << '\n';
        }
        //std::cout << leituras.substr(pos, posf - pos) << '\n';
        else if(linha.find('<') != std::string::npos)
        {
            //1_ <0811413+1194753+****d090_*P_,1.600_
            //1_
            size_t i = 0, j = linha.find('_');
            std::cout << linha.substr(i, j - i) << '\t';
            //<0811413+
            i = linha.find('<') + 1;
            j = linha.find('+', i);
            std::cout << linha.substr(i, j - i) << '\t';
            //+1194753+
            i = linha.find('+') + 1;
            j = linha.find('+', i);
            std::cout << linha.substr(i, j - i) << '\t';
            //_*P_,
            i = linha.find("_*") + 2;
            j = linha.find("_,", i);
            std::cout << linha.substr(i, j - i) << '\t';
            i = linha.find("_,") + 2;
            std::cout << linha.substr(i) << '\n';
        }
        else
        {
            //1_ ?+00023023m0811410+1194753d+00022754***+25-30050_*P_,1.600
            //1_
            size_t i = 0, j = linha.find('_');
            std::cout << linha.substr(i, j - i) << '\t';
            //?+00023023m
            i = linha.find("?+") + 2;
            j = linha.find('m', i);
            std::cout << linha.substr(i, j - i) << '\t';
            //m0811410+
            i = linha.find('m') + 1;
            j = linha.find('+', i);
            std::cout << linha.substr(i, j - i) << '\t';
            //+1194753d+
            i = linha.find('+', i) + 1;
            j = linha.find("d+", i);
            std::cout << linha.substr(i, j - i) << '\t';
            //_*P_
            i = linha.find("_*", i) + 2;
            j = linha.find('_', i);
            std::cout << linha.substr(i, j - i) << '\t';
            //_,1.600
            i = linha.find("_,", i) + 2;
            std::cout << linha.substr(i) << '\n';
        }
        //else std::cout << linha << '\n';
    }
    return 0;
}
