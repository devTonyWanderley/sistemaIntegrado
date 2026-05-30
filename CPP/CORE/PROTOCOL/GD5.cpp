//	GD5.cpp
#include "GD5.hpp"
#include <sstream>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <algorithm>

std::string GD5::Gd5::lerTmp(const std::filesystem::path &fonte)
{
    std::ifstream arquivo(fonte, std::ios::binary);
    if(!arquivo.is_open()) return "";
    std::stringstream ss;
    ss << arquivo.rdbuf();
    std::string lido = ss.str();
    if(lido.empty()) return lido;
    std::string r = "";
    r.reserve(lido.size());
    size_t pos = 1;
    while(pos < lido.size())
    {
        size_t n = (pos + 128 < lido.size())? 128 : lido.size() - pos;
        r.append(lido.substr(pos, n));
        pos += 136;
    }
    return r;
}

std::string GD5::Gd5::truncarTexto(std::string &texto)
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

std::vector<std::string> GD5::Gd5::fatiar(std::string &texto)
{
    if(texto.empty()) return {};
    std::vector<std::string> r;
    size_t posi = 0, posf = texto.find('_');
    //  1_(P_)1.676 |
    //  1_ <0811413+1194753+****d090_*P_,1.600 |
    //  1_ ?+00033870m0812338+1205111d+00033488***+25-30058_*P_,1.600
    r.push_back(texto.substr(posi, posf - posi));   //  {"1"}
    if(texto.find("_(") == std::string::npos)
    {
        size_t ast = texto.find("_*");
        if(ast == std::string::npos) return r;
        posi = ast + 2;
        posf = texto.find('_', posi);
        if(posf == std::string::npos) return r;
        r.push_back(texto.substr(posi, posf - posi));   //  {"1" "P"}
        ast = texto.find("_,");
        if(ast == std::string::npos) return r;
        posi = ast + 2;
        r.push_back(texto.substr(posi));   //  {"1" "P" "1.600"}
        if(texto.find("?+") == std::string::npos)
        {
            ast = texto.find('<');
            if(ast == std::string::npos) return r;
            posi = ast + 1;
            posf = texto.find('+', posi);
            r.push_back(texto.substr(posi, posf - posi));   //  {"1" "P" "1.600" "0811413"}
            posi = posf + 1;
            posf = texto.find('+', posi);
            if(posf == std::string::npos) return r;
            r.push_back(texto.substr(posi, posf - posi));   //  {"1" "P" "1.600" "0811413" "1194753"}
            return r;   //  {id atr hs av ah}
        }
        ast = texto.find("?+");
        if(ast == std::string::npos) return r;
        posi = ast + 2;
        posf = texto.find('m', posi);
        if(posf == std::string::npos) return r;
        r.push_back(texto.substr(posi, posf - posi));   //  {"1" "P" "1.600" "00033870"}
        posi = posf + 1;
        posf = texto.find('+', posi);
        if(posf == std::string::npos) return r;
        r.push_back(texto.substr(posi, posf - posi));   //  {"1" "P" "1.600" "00033870" "0812338"}
        posi = posf + 1;
        posf = texto.find("d+", posi);
        if(posf == std::string::npos) return r;
        r.push_back(texto.substr(posi, posf - posi));   //  {"1" "P" "1.600" "00033870" "0812338" "1205111"}
        return r;   //  {id atr hs di av ah}
    }
    size_t abrep = texto.find("_(");
    if(abrep == std::string::npos) return r;
    posi = abrep + 2;
    posf = texto.find('_', posi);
    if(posf == std::string::npos) return r;
    r.push_back(texto.substr(posi, posf - posi));   //  {"1" "P"}
    posi = posf + 2;
    if(posi < texto.length()) r.push_back(texto.substr(posi));   //  {"1" "P" "1.676"}
    return r;   //  {id atr hi}
}

bool GD5::Gd5::Ler(const std::filesystem::path &fonte)
{
    std::string lido = lerTmp(fonte);
    if(lido.empty()) return false;
    while(!lido.empty())
    {
        std::string tx = truncarTexto(lido);
        std::vector<std::string> reg = fatiar(tx);
        if(reg.size() < 3) continue;
        Leitura l;
        std::snprintf(l.nome, sizeof(l.nome), "%s", reg.at(0).c_str());
        std::snprintf(l.atri, sizeof(l.atri), "%s", reg.at(1).c_str());
        try
        {
            std::string s = reg.at(2), ss;
            for(auto j : s) if(j != '.') ss.push_back(j);
            if(!ss.empty()) l.altu = 10 * static_cast<uint16_t>(std::stoi(ss));
            if(reg.size() >= 6)
            {
                l.dist = 10 * static_cast<uint32_t>(std::stoi(reg.at(3)));
                std::string gr = reg.at(4).substr(0, 3), mi = reg.at(4).substr(3,2), se = reg.at(4).substr(5,2);
                l.aVer = static_cast<uint32_t>(std::stoi(se)) + (60 * static_cast<uint32_t>(std::stoi(mi))) +
                         (3600 * static_cast<uint32_t>(std::stoi(gr)));
                gr = reg.at(5).substr(0,3); mi = reg.at(5).substr(3,2); se = reg.at(5).substr(5,2);
                l.aHor = static_cast<uint32_t>(std::stoi(se)) + (60 * static_cast<uint32_t>(std::stoi(mi))) +
                         (3600 * static_cast<uint32_t>(std::stoi(gr)));
            }
            else if(reg.size() == 5)
            {
                if(reg.at(3).length() >= 7 && reg.at(4).length() >= 7)
                {
                    std::string gr = reg.at(3).substr(0, 3), mi = reg.at(3).substr(3,2), se = reg.at(3).substr(5,2);
                    l.aVer = static_cast<uint32_t>(std::stoi(se)) + (60 * static_cast<uint32_t>(std::stoi(mi))) +
                             (3600 * static_cast<uint32_t>(std::stoi(gr)));
                    gr = reg.at(4).substr(0,3); mi = reg.at(4).substr(3,2); se = reg.at(4).substr(5,2);
                    l.aHor = static_cast<uint32_t>(std::stoi(se)) + (60 * static_cast<uint32_t>(std::stoi(mi))) +
                             (3600 * static_cast<uint32_t>(std::stoi(gr)));
                }
            }
            mCaderneta.push_back(l);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Erro ao processar: " << e.what() << std::endl;
        }
    }
    return true;
}

bool GD5::Gd5::Salvar(const std::filesystem::path &destino)
{
    if(mCaderneta.empty()) return false;
    std::ofstream arquivo(destino, std::ios::out | std::ios::binary);
    if(!arquivo.is_open()) return false;
    arquivo.write(reinterpret_cast<const char*>(mCaderneta.data()), mCaderneta.size() * sizeof(Leitura));
    arquivo.close();
    return true;
}