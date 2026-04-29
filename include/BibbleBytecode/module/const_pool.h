// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_MODULE_CONST_POOL_H
#define BIBBLEBYTECODE_MODULE_CONST_POOL_H 1

#include <cstdint>
#include <string_view>

namespace bibblebytecode {
    using ConstantIndex = uint16_t;

    class ConstPool {
    public:
        enum Tag : uint8_t {
            BYTE = 0x01,
            SHORT = 0x02,
            INT = 0x03,
            LONG = 0x04,
            STRING = 0x05,
            MODULE_INFO = 0x06,
            CLASS_INFO = 0x07,
            FIELD_INFO = 0x08,
            METHOD_INFO = 0x09,
            FUNCTION_INFO = 0x0A,
        };

        struct ModuleInfo {
            ConstantIndex name;
        };

        struct ClassInfo {
            ConstantIndex module;
            ConstantIndex name;
        };

        struct FieldInfo {
            ConstantIndex clas;
            ConstantIndex name;
        };

        struct MethodInfo {
            ConstantIndex clas;
            ConstantIndex name;
        };

        struct FunctionInfo {
            ConstantIndex module;
            ConstantIndex name;
        };

        struct Entry {
            Tag tag;
            union {
                int8_t b;
                int16_t s;
                int32_t i;
                int64_t l;
                std::string_view str;
                ModuleInfo mi;
                ClassInfo ci;
                FieldInfo fi;
                MethodInfo mei;
                FunctionInfo fni;
            } u;
        };

        ConstPool() = default;

        ConstPool(uint16_t entryCount, Entry* entries)
            : mEntryCount(entryCount)
            , mEntries(entries) {}

        uint16_t getEntryCount() const { return mEntryCount; }
        Entry* getEntries() const { return mEntries; }

    private:
        uint16_t mEntryCount;
        Entry* mEntries;
    };
}

#endif //BIBBLEBYTECODE_MODULE_CONST_POOL_H