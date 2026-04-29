// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_MODULE_MODULE_H
#define BIBBLEBYTECODE_MODULE_MODULE_H 1

#include "BibbleBytecode/module/class.h"
#include "BibbleBytecode/module/function.h"

#include <cstdint>

namespace bibblebytecode {
    constexpr uint32_t MAGIC = 0xFAAC6767;

    struct Module {
        uint32_t magic;
        uint16_t formatVersion;

        ConstantIndex name;

        ConstPool constPool;

        uint16_t classCount;
        uint16_t functionCount;

        Class* classes;
        Function* functions;
    };
}

#endif //BIBBLEBYTECODE_MODULE_MODULE_H