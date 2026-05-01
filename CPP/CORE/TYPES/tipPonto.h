//  tipPonto.h
#pragma once
#include <cstdint>

struct alignas(16) Ponto
{
    uint32_t abci;
    uint32_t orde;
    uint32_t cota;
    uint32_t iMeta;
};

struct alignas(8) MetaPonto
{
    uint32_t iNome;
    uint32_t iAtri;
};