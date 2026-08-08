//  C:\Tony\Soft\cpp\integrado\sistemaIntegrado\CPP\CORE\PROTOCOL\GeodeticGd5.cpp, substituindo ..\GD5.cpp
#include "GeodeticGd5.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

std::string Gdtc::Geodetic::lerTmp(const std::filesystem::path &fonte)
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

std::string Gdtc::Geodetic::truncarTexto(std::string &texto)
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

std::vector<std::string> Gdtc::Geodetic::fatiar(std::string &texto)
{
    if(texto.empty()) return {};
    std::vector<std::string> r;
    size_t posi = 0, posf = texto.find('_');
    r.push_back(texto.substr(posi, posf - posi));
    if(texto.find("_(") == std::string::npos)
    {
        size_t ast = texto.find("_*");
        if(ast == std::string::npos) return r;
        posi = ast + 2;
        posf = texto.find('_', posi);
        if(posf == std::string::npos) return r;
        r.push_back(texto.substr(posi, posf - posi));
        ast = texto.find("_,");
        if(ast == std::string::npos) return r;
        posi = ast + 2;
        r.push_back(texto.substr(posi));
        if(texto.find("?+") == std::string::npos)
        {
            ast = texto.find('<');
            if(ast == std::string::npos) return r;
            posi = ast + 1;
            posf = texto.find('+', posi);
            r.push_back(texto.substr(posi, posf - posi));
            posi = posf + 1;
            posf = texto.find('+', posi);
            if(posf == std::string::npos) return r;
            r.push_back(texto.substr(posi, posf - posi));
            return r;
        }
        ast = texto.find("?+");
        if(ast == std::string::npos) return r;
        posi = ast + 2;
        posf = texto.find('m', posi);
        if(posf == std::string::npos) return r;
        r.push_back(texto.substr(posi, posf - posi));
        posi = posf + 1;
        posf = texto.find('+', posi);
        if(posf == std::string::npos) return r;
        r.push_back(texto.substr(posi, posf - posi));
        posi = posf + 1;
        posf = texto.find("d+", posi);
        if(posf == std::string::npos) return r;
        r.push_back(texto.substr(posi, posf - posi));
        return r;
    }
    size_t abrep = texto.find("_(");
    if(abrep == std::string::npos) return r;
    posi = abrep + 2;
    posf = texto.find('_', posi);
    if(posf == std::string::npos) return r;
    r.push_back(texto.substr(posi, posf - posi));
    posi = posf + 2;
    if(posi < texto.length()) r.push_back(texto.substr(posi));
    return r;
}

bool Gdtc::Geodetic::Ler(const std::filesystem::path &fonte)
{
    std::string lido = lerTmp(fonte);
    if(lido.empty()) return false;
    while(!lido.empty())
    {
        std::string tx = truncarTexto(lido);
        std::vector<std::string> reg = fatiar(tx);
        if(reg.size() < 3) continue;
        Geom::LeituraGD5 l;
        l.nome = reg.at(0);
        l.atri = reg.at(1);
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
            }   //  fim do if
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
            }  //  fim do else if
            mCaderneta.push_back(l);
        }   //  fim do try
        catch(const std::exception& e)
        {
            std::cerr << "Erro ao processar: " << e.what() << std::endl;
        } //  fim do try-catch
    }  //  fim do while
    return (!mCaderneta.empty());
}

void Gdtc::Geodetic::CalcularCaderneta()
{
    if(mCaderneta.empty()) return;
    mPontos.clear();
    mPontos.reserve(mCaderneta.size());
    Geom::PontoMetrico* pEst = nullptr;
    double hi = 0;
    for(Geom::LeituraGD5& l : mCaderneta)
    {
        if(l.aHor == std::numeric_limits<uint32_t>::max())
        {
            hi = l.altu;
            hi /= 1000;
            if(pEst)
            {
                for(Geom::PontoMetrico& pc : mPontos)
                {
                    if(l.nome == pc.nome)
                    {
                        pEst = &pc;
                        break;
                    }   //  fim do if
                }   //  fim do for
            }   //  fim do if
            else
            {
                Geom::PontoMetrico p;
                p.nome = l.nome;
                p.atri = l.atri;
                p.x = p.y = p.z = 0.0;
                mPontos.push_back(p);
                pEst = mPontos.data();
            }  //  fim do else
        }   //  fim do if
        else if(l.dist != std::numeric_limits<uint32_t>::max())
        {
            Geom::PontoMetrico p;
            p.nome = l.nome;
            p.atri = l.atri;
            double ah = l.aHor, av = l.aVer, dist = l.dist / 1000.0, hs = l.altu;
            ah /= 648000;
            ah *= M_PI;
            av /= 648000;
            av *= M_PI;
            hs /= 1000;
            p.z = pEst->z + hi - hs + (dist * cos(av));
            p.y = pEst->y + (dist * sin(av) * cos(ah));
            p.x = pEst->x + (dist * sin(av) * sin(ah));
            mPontos.push_back(p);
        }   //  fim do else if
    }  //  fim do for
}