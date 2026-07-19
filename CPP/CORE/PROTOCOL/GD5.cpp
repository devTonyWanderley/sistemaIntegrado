//	GD5.cpp
#include "GD5.hpp"
#include <sstream>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <bit>

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
            if(!ss.empty()) l.altu = static_cast<uint16_t>(std::stoi(ss));
            if(reg.size() >= 6)
            {
                l.dist = static_cast<uint32_t>(std::stoi(reg.at(3)));
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
    if(mPontos.empty()) return false;
    std::ofstream arquivo(destino, std::ios::out | std::ios::binary);
    if(!arquivo.is_open()) return false;
    arquivo.write(reinterpret_cast<const char*>(mPontos.data()), mPontos.size() * sizeof(PontoNormalizado));
    arquivo.close();
    return true;
}

bool GD5::Gd5::CarregarCad(const std::filesystem::path &fonte)
{
    std::ifstream arquivo(fonte, std::ios::in | std::ios::binary);
    if(!arquivo.is_open()) return false;
    Leitura registro;
    mCaderneta.clear();
    while(arquivo.read(reinterpret_cast<char*>(&registro), sizeof(Leitura))) mCaderneta.push_back(registro);
    arquivo.close();
    return true;
}

void GD5::Gd5::CalcularCaderneta()
{
    if(mCaderneta.empty()) return;
    //std::vector<PontoCalculado> pontos;
    mPCal.clear();
    //pontos.reserve(mCaderneta.size());
    mPCal.reserve(mCaderneta.size());
    PontoCalculado* pEst = nullptr;
    double hi = 0;
    for(Leitura& l : mCaderneta)
    {
        if(l.aHor == std::numeric_limits<uint32_t>::max())  //  atualizar estação
        {
            hi = l.altu;
            hi /= 1000;
            if(pEst)
            {
                for(PontoCalculado& pc : mPCal)
                {
                    for(int i = 0; i < 13; i++)
                    {
                        if(l.nome[i] != pc.id[i]) break;
                        if(i == 12) pEst = &pc;
                    }
                }
            }
            else
            {
                PontoCalculado p;
                for(int i = 0; i < 13; i++)
                {
                    p.id[i] = l.nome[i];
                    p.cod[i] = l.atri[i];
                }
                p.x = p.y = p.z = 0;
                mPCal.push_back(p);
                pEst = mPCal.data();
            }
        }
        else if(l.dist != std::numeric_limits<uint32_t>::max())
        {
            PontoCalculado p;
            for(int i = 0; i < 13; i++)
            {
                p.id[i] = l.nome[i];
                p.cod[i] = l.atri[i];
            }
            double ah = l.aHor, av = l.aVer, dist = l.dist / 1000.0, hs = l.altu;
            ah /= 648000;
            ah *= M_PI;
            av /= 648000;
            av *= M_PI;
            hs /= 1000;
            p.z = pEst->z + hi - hs + (dist * cos(av));
            p.y = pEst->y + (dist * sin(av) * cos(ah));
            p.x = pEst->x + (dist * sin(av) * sin(ah));
            mPCal.push_back(p);
        }
    }
    /*  Daqui pra baixo, deve-se organizar a octotree
    double
        xMin = std::numeric_limits<double>::max(),
        yMin = std::numeric_limits<double>::max(),
        zMin = std::numeric_limits<double>::max(),
        xMax = std::numeric_limits<double>::min(),
        yMax = std::numeric_limits<double>::min(),
        zMax = std::numeric_limits<double>::min();
    for(PontoCalculado& p : pontos)
    {
        xMin = (p.x < xMin)? p.x : xMin;
        yMin = (p.y < yMin)? p.y : yMin;
        zMin = (p.z < zMin)? p.z : zMin;
        xMax = (p.x > xMax)? p.x : xMax;
        yMax = (p.y > yMax)? p.x : yMax;
        zMax = (p.z > zMax)? p.x : zMax;
    }
    //  Aqui entra o recálculo das constantes
    //  #1 -> extremos superiores da octotree:
    uint32_t
        xOtMax = std::bit_ceil(reinterpret_cast<uint32_t>(10000 * (xMax - xMin))),
        yOtMax = std::bit_ceil(reinterpret_cast<uint32_t>(10000 * (yMax - yMin))),
        zOtMax = std::bit_ceil(reinterpret_cast<uint32_t>(10000 * (zMax - zMin)));
    //  #2 -> Deslocamento da orígem (constante de soma):
    uint32_t
        deltaX = (xOtMax - (reinterpret_cast<uint32_t>(10000 * (xMax - xMin)))) >> 1,   //  folga
        deltaY = (yOtMax - (reinterpret_cast<uint32_t>(10000 * (yMax - yMin)))) >> 1,
        deltaZ = (zOtMax - (reinterpret_cast<uint32_t>(10000 * (zMax - zMin)))) >> 1;
    //  !!! por enquanto, o arquivo e mPontos ainda não organizados como octotree, mas já com valores centralizados !!!

    mPontos.clear();
    mPontos.reserve(pontos.size());
    for(PontoCalculado& p : pontos)
    {
        PontoNormalizado pn;
        for(int i = 0; i < 13; i++)
        {
            pn.nome[i] = p.id[i];
            pn.atri[i] = p.cod[i];
        }
        double x = p.x - xMin, y = p.y - yMin, z = p.z - zMin;
        pn.abci = (10000 * x) + deltaX;
        pn.orde = (10000 * y) + deltaY;
        pn.cota = (10000 * z) + deltaZ;
        mPontos.push_back(pn);
    }
    */
}