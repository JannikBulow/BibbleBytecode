// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_MODULE_FIELD_H
#define BIBBLEBYTECODE_MODULE_FIELD_H 1

#include "BibbleBytecode/module/const_pool.h"

namespace bibblebytecode {
    struct Field {
        uint8_t typeID;
        uint8_t padding_;
        ConstantIndex name;
    };
}

#endif // BIBBLEBYTECODE_MODULE_FIELD_H
