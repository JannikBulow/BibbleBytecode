// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE__COMPATABILITY_H
#define BIBBLEBYTECODE__COMPATABILITY_H 1

#include <cstdint>

// this is why i love c++
//TODO: add a macro or something to disallow intrinsics when building bibblebytecode
namespace bibblebytecode::compat {
    inline uint16_t bswap16(uint16_t v) {
#if defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap16(v);
#elif defined(_MSC_VER)
        return _byteswap_ushort(v);
#else
        return (v >> 8) | (v << 8);
#endif
    }

    inline uint32_t bswap32(uint32_t v) {
#if defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap32(v);
#elif defined(_MSC_VER)
        return _byteswap_ulong(v);
#else
        return (v >> 24) | ((v >> 8) & 0x0000FF00) | ((v << 8) & 0x00FF0000) | (v << 24);
#endif
    }

    inline uint64_t bswap64(uint64_t v) {
#if defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap64(v);
#elif defined(_MSC_VER)
        return _byteswap_uint64(v);
#else
        return (v >> 56) |
           ((v >> 40) & 0x000000000000FF00ULL) |
           ((v >> 24) & 0x0000000000FF0000ULL) |
           ((v >>  8) & 0x00000000FF000000ULL) |
           ((v <<  8) & 0x000000FF00000000ULL) |
           ((v << 24) & 0x0000FF0000000000ULL) |
           ((v << 40) & 0x00FF000000000000ULL) |
           (v << 56);
#endif
    }
}

#endif //BIBBLEBYTECODE__COMPATABILITY_H