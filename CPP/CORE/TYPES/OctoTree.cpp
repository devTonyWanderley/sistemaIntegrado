#include "OctoTree.hpp"
#include <algorithm>

#include <iostream>

unsigned __int128 OT::OctoTree::espalhaBits3d(std::uint64_t n)
{
    unsigned __int128 x = n & 0x3FFFFFFFFFFULL;
    x = (x | (x << 32)) & ((unsigned __int128)(0x000003FF) << 64 | 0x00000000FFFFFFFFULL);
    x = (x | (x << 16)) & ((unsigned __int128)(0x0000C000) << 64 | 0x00FF000000FF0000ULL);
    x = (x | (x << 8))  & ((unsigned __int128)(0x00030000) << 64 | 0x0F00F00F00F00F00ULL);
    x = (x | (x << 4))  & ((unsigned __int128)(0x000C0000) << 64 | 0x30C30C30C30C30C3ULL);
    x = (x | (x << 2))  & ((unsigned __int128)(0x00100000) << 64 | 0x9249249249249249ULL);
    return x;
}

unsigned __int128 OT::OctoTree::geraMorton3d(std::uint64_t x, std::uint64_t y, std::uint64_t z)
{
    return (espalhaBits3d(x) | (espalhaBits3d(y) << 1) | (espalhaBits3d(z) << 2));
}

std::uint64_t OT::OctoTree::compctaBits(unsigned __int128 m)
{
    m &= (static_cast<unsigned __int128>(0x00100000) << 64 | 0x9249249249249249ULL);
    m = (m | (m >> 2))  & (static_cast<unsigned __int128>(0x000C0000) << 64 | 0x30C30C30C30C30C3ULL);
    m = (m | (m >> 4))  & (static_cast<unsigned __int128>(0x00030000) << 64 | 0x0F00F00F00F00F00ULL);
    m = (m | (m >> 8))  & (static_cast<unsigned __int128>(0x0000C000) << 64 | 0x00FF000000FF0000ULL);
    m = (m | (m >> 16)) & (static_cast<unsigned __int128>(0x000003FF) << 64 | 0x00000000FFFFFFFFULL);
    m = (m | (m >> 32));
    return static_cast<std::uint64_t>(m & 0x3FFFFFFFFFFULL);
}

double OT::OctoTree::lerCoordi(int i, unsigned __int128 m)
{
    if((i < 0) || (i > 2)) return std::numeric_limits<double>::max();
    double v;
    if(i < 1) v = static_cast<double>(compctaBits(m));
    else if(i > 1) v = static_cast<double>(compctaBits((m >> 2)));
    else v = static_cast<double>(compctaBits((m >> 1)));
    v -= mOffSet[i];
    return (v / mEscala[i]);
}

void OT::OctoTree::carregaOT(std::vector<Geom::PontoMetrico>& pontos)
{
    std::cout << "DEBUG: Iniciando carregaOT. Tamanho de pontos: " << pontos.size() << std::endl;
    mDict.reserve(2 * pontos.size());
    double
        xdMin = std::numeric_limits<double>::max(),
        xdMax = std::numeric_limits<double>::lowest(),
        ydMin = std::numeric_limits<double>::max(),
        ydMax = std::numeric_limits<double>::lowest(),
        zdMin = std::numeric_limits<double>::max(),
        zdMax = std::numeric_limits<double>::lowest();
    std::cout << "x [" << xdMin << ' ' << xdMax << ']' << std::endl;
    std::cout << pontos[5].x << std::endl;
    for(size_t i = 0; i < (pontos.size() - 1); i++)
    {
        std::cout << pontos[i].nome << ' ' << pontos[i].atri << std::endl;
        mDict.push_back(pontos[i].nome);
        mDict.push_back(pontos[i].atri);
    }
    std::cout << "\n\t67\n";
    for(size_t i = 0; i < (pontos.size() - 1); i++)
    {
        std::cout << pontos[i].x << ' ' << pontos[i].y << ' ' << pontos[i].z << std::endl;
        xdMin = (pontos[i].x < xdMin)? pontos[i].x : xdMin;
        xdMax = (pontos[i].x > xdMax)? pontos[i].x : xdMax;
        ydMin = (pontos[i].y < ydMin)? pontos[i].y : ydMin;
        ydMax = (pontos[i].y > ydMax)? pontos[i].y : ydMax;
        zdMin = (pontos[i].z < zdMin)? pontos[i].z : zdMin;
        zdMax = (pontos[i].z > zdMax)? pontos[i].z : zdMax;
    }
    std::cout << "\n\t74\n";
    /*
    for(Geom::PontoMetrico& p : pontos)
    {
        std::cout << "\n\t58\n";
        mDict.push_back(p.nome);
        mDict.push_back(p.atri);
        xdMin = (p.x < xdMin)? p.x : xdMin;
        xdMax = (p.x > xdMax)? p.x : xdMax;
        ydMin = (p.y < ydMin)? p.y : ydMin;
        ydMax = (p.y > ydMax)? p.y : ydMax;
        zdMin = (p.z < zdMin)? p.z : zdMin;
        zdMax = (p.z > zdMax)? p.z : zdMax;
    }
    */
    std::sort(mDict.begin(), mDict.end());
    auto it = std::unique(mDict.begin(), mDict.end());
    mDict.erase(it, mDict.end());
    mEscala[0] = mEscala[1] = mEscala[2] = static_cast<double>(1ULL << 42) - 2.0;
    mEscala[0] /= (xdMax - xdMin);
    mEscala[1] /= (ydMax - ydMin);
    mEscala[2] /= (zdMax - zdMin);
    mOffSet[0] = -((xdMin * mEscala[0]) - 1.0);
    mOffSet[1] = -((ydMin * mEscala[1]) - 1.0);
    mOffSet[2] = -((zdMin * mEscala[2]) - 1.0);
    mCoords.reserve(pontos.size());
    for(Geom::PontoMetrico& p : pontos)
        mCoords.push_back(geraMorton3d(static_cast<std::uint64_t>((p.x * mEscala[0]) + mOffSet[0]),
                                       static_cast<std::uint64_t>((p.y * mEscala[1]) + mOffSet[1]),
                                       static_cast<std::uint64_t>((p.z * mEscala[2]) + mOffSet[2])));
    std::sort(mCoords.begin(), mCoords.end());
    auto ti = std::unique(mCoords.begin(), mCoords.end());
    mCoords.erase(ti, mCoords.end());
    mIPontos.reserve(mCoords.size());
    for(Geom::PontoMetrico& p : pontos)
    {
        std::uint32_t indice = buscaBinaria(mDict, p.nome);
        if(indice == std::numeric_limits<std::uint32_t>::max()) continue;
        Geom::iPonto ip;
        ip.iNome = indice;
        indice = buscaBinaria(mDict, p.atri);
        if(indice == std::numeric_limits<std::uint32_t>::max()) continue;
        ip.iAtri = indice;
        unsigned __int128 m = geraMorton3d(static_cast<std::uint64_t>((p.x * mEscala[0]) + mOffSet[0]),
                                           static_cast<std::uint64_t>((p.y * mEscala[1]) + mOffSet[1]),
                                           static_cast<std::uint64_t>((p.z * mEscala[2]) + mOffSet[2]));
        indice = buscaBinaria(mCoords, m);
        if(indice == std::numeric_limits<std::uint32_t>::max()) continue;
        ip.iMorton = indice;
        mIPontos.push_back(ip);
    }
}