// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_MODULE_METHOD_H
#define BIBBLEBYTECODE_MODULE_METHOD_H 1

#include "BibbleBytecode/module/const_pool.h"

namespace bibblebytecode {
    struct Method {
        ConstantIndex name;
        ConstantIndex function;
    };
}

#endif // BIBBLEBYTECODE_MODULE_METHOD_H
