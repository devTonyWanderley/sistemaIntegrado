//  tipPonto.h
#pragma once
#include <cstdint>

struct alignas(16) PontoBin
{
    uint32_t abci, orde, cota;
    union
    {
        struct
        {
            uint16_t iNome, iAtri;
        };
        uint32_t iMeta;
    };
};