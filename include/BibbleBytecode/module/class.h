// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_MODULE_CLASS_H
#define BIBBLEBYTECODE_MODULE_CLASS_H 1

#include "BibbleBytecode/module/field.h"
#include "BibbleBytecode/module/method.h"

namespace bibblebytecode {
    struct Class {
        ConstantIndex name;
        ConstantIndex superClass;

        uint16_t fieldCount;
        uint16_t methodCount;

        Field* fields;
        Method* methods;
    };
}

#endif // BIBBLEBYTECODE_MODULE_CLASS_H
