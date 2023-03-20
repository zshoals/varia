#pragma once

#include "varia/Vcommon.hpp"

#include <stdint.h>
#include <string.h>

#define c_strhash_ROTL(x, k) (x << (k) | x >> (8*sizeof(x) - (k)))

VARIA_INLINE constexpr uint64_t cfasthash(const void* key, intptr_t len) {
    const uint8_t *x = (const uint8_t*) key;
    uint64_t u8 = 0, h = 1; intptr_t n = len >> 3;
    uint32_t u4 = 0;
    while (n--) {
        memcpy(&u8, x, 8), x += 8;
        h += (c_strhash_ROTL(u8, 26) ^ u8)*0xc6a4a7935bd1e99d;
    }
    switch (len &= 7) {
        case 0: return h;
        case 4: memcpy(&u4, x, 4);
                return h + u4*0xc6a4a7935bd1e99d;
    }
    h += *x++;
    while (--len) h = (h << 10) - h + *x++;
    return c_strhash_ROTL(h, 26) ^ h;
}

VARIA_INLINE constexpr uint64_t cstr_hash(void const * key)
{
    return cfasthash(key, strlen((char const * )key));
}