// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_MODULE_FIELD_H
#define BIBBLEBYTECODE_MODULE_FIELD_H 1

#include "BibbleBytecode/module/const_pool.h"

namespace bibblebytecode {
    struct Field {
        uint32_t typeID;
        ConstantIndex name;
    };
}

#endif // BIBBLEBYTECODE_MODULE_FIELD_H
