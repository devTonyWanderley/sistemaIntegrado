#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

class DicionarioTextos
{
public:
    explicit DicionarioTextos(uint16_t estimativa = 10)
    {
        mLista.reserve(estimativa);
        mMapaInverso.reserve(estimativa);
        adicionar("");
    }

    uint16_t adicionar(const std::string& texto)
    {
        if (texto.empty()) return 0;
        auto it = mMapaInverso.find(texto);
        if (it != mMapaInverso.end()) return it->second;
        if (mLista.size() >= 0xFFFF) return 0;
        uint16_t novoIndice = static_cast<uint16_t>(mLista.size());
        mLista.push_back(texto);
        mMapaInverso[texto] = novoIndice;
        return novoIndice;
    }

    std::string obter(uint16_t indice) const {return (indice < mLista.size()) ? mLista[indice] : "";}

private:
    std::vector<std::string> mLista;
    std::unordered_map<std::string, uint16_t> mMapaInverso;
};
