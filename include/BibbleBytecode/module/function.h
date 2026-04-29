// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEVM_CORE_MODULE_FUNCTION_H
#define BIBBLEVM_CORE_MODULE_FUNCTION_H 1

#include "BibbleBytecode/module/const_pool.h"

#include <cstdint>

namespace bibblebytecode {
    enum FunctionFlags : uint16_t {
        FUNC_NATIVE = 0x0001,
    };

    struct Function {
        ConstantIndex name;
        uint16_t flags;
        uint16_t registerCount;
        uint16_t parameterCount;
        uint32_t bytecodeSize;
        const uint8_t* bytecode;
    };
}

#endif // BIBBLEVM_CORE_MODULE_FUNCTION_H
