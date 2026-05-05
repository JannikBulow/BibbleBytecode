// Copyright 2026 Jannik Laugmand Bülow

#include "BibbleBytecode/_compatability.h"
#include "BibbleBytecode/writer.h"

namespace bibblebytecode::writer {
    template<class T>
    bool WriteLE(WritableByteBuffer& out, T value) {
        static_assert(std::is_integral_v<T>, "T must be integral");

        // just in case yo
        if constexpr (std::endian::native == std::endian::big) {
            // ReSharper disable once CppDFAUnreachableCode
            if constexpr (sizeof(T) == 2) value = compat::bswap16(value);
            if constexpr (sizeof(T) == 4) value = compat::bswap32(value);
            if constexpr (sizeof(T) == 8) value = compat::bswap64(value);
        }

        out.write(reinterpret_cast<uint8_t*>(&value), sizeof(T));
        if (out.fail()) return false;

        return true;
    }

    template<class Len>
    bool WriteString(WritableByteBuffer& out, const char* str, Len len) {
        if (!WriteLE<Len>(out, len)) return false;

        out.write(reinterpret_cast<const uint8_t*>(str), len);
        return !out.fail();
    }

    bool WriteConstantIndex(WritableByteBuffer& out, ConstantIndex index) {
        return WriteLE<ConstantIndex>(out, index);
    }

    bool WriteConstantTag(WritableByteBuffer& out, ConstPool::Tag tag) {
        return WriteLE<uint8_t>(out, static_cast<uint8_t>(tag));
    }

    bool WriteModuleInfo(WritableByteBuffer& out, const ConstPool::ModuleInfo& moduleInfo) {
        return WriteLE<ConstantIndex>(out, moduleInfo.name);
    }

    bool WriteClassInfo(WritableByteBuffer& out, const ConstPool::ClassInfo& classInfo) {
        if (!WriteLE<ConstantIndex>(out, classInfo.module)) return false;
        return WriteLE<ConstantIndex>(out, classInfo.name);
    }

    bool WriteFieldInfo(WritableByteBuffer& out, const ConstPool::FieldInfo& fieldInfo) {
        if (!WriteLE<ConstantIndex>(out, fieldInfo.clas)) return false;
        return WriteLE<ConstantIndex>(out, fieldInfo.name);

    }

    bool WriteMethodInfo(WritableByteBuffer& out, const ConstPool::MethodInfo& methodInfo) {
        if (!WriteLE<ConstantIndex>(out, methodInfo.clas)) return false;
        return WriteLE<ConstantIndex>(out, methodInfo.name);
    }

    bool WriteFunctionInfo(WritableByteBuffer& out, const ConstPool::FunctionInfo& functionInfo) {
        if (!WriteLE<ConstantIndex>(out, functionInfo.module)) return false;
        return WriteLE<ConstantIndex>(out, functionInfo.name);
    }

    bool WriteConstantEntry(WritableByteBuffer& out, const ConstPool::Entry& entry) {
        switch (entry.tag) {
            case ConstPool::BYTE: return WriteLE<int8_t>(out, entry.u.b);
            case ConstPool::SHORT: return WriteLE<int16_t>(out, entry.u.s);
            case ConstPool::INT: return WriteLE<int32_t>(out, entry.u.i);
            case ConstPool::LONG: return WriteLE<int64_t>(out, entry.u.l);
            case ConstPool::STRING: return WriteString<uint32_t>(out, entry.u.str.data(), entry.u.str.size());
            case ConstPool::MODULE_INFO: return WriteModuleInfo(out, entry.u.mi);
            case ConstPool::CLASS_INFO: return WriteClassInfo(out, entry.u.ci);
            case ConstPool::FIELD_INFO: return WriteFieldInfo(out, entry.u.fi);
            case ConstPool::METHOD_INFO: return WriteMethodInfo(out, entry.u.mei);
            case ConstPool::FUNCTION_INFO: return WriteFunctionInfo(out, entry.u.fni);
        }
        return false;
    }

    bool WriteConstantTagAndEntry(WritableByteBuffer& out, const ConstPool::Entry& entry) {
        if (!WriteConstantTag(out, entry.tag)) return false;
        return WriteConstantEntry(out, entry);
    }

    bool WriteConstantEntryCount(WritableByteBuffer& out, uint16_t entryCount) {
        return WriteLE<uint16_t>(out, entryCount);
    }

    bool WriteConstPool(WritableByteBuffer& out, const ConstPool& pool) {
        if (!WriteConstantEntryCount(out, pool.getEntryCount())) return false;

        for (uint16_t i = 1; i < pool.getEntryCount(); i++) {
            if (!WriteConstantTagAndEntry(out, pool.getEntries()[i])) return false;
        }

        return true;
    }

    bool WriteFieldCount(WritableByteBuffer& out, uint16_t fieldCount) {
        return WriteLE<uint16_t>(out, fieldCount);
    }

    bool WriteField(WritableByteBuffer& out, const Field& field) {
        if (!WriteLE<uint8_t>(out, field.typeID)) return false;
        if (!WriteLE<uint8_t>(out, field.padding_)) return false; // should padding_ be written or just 0?
        return WriteConstantIndex(out, field.name);
    }

    bool WriteMethodCount(WritableByteBuffer& out, uint16_t methodCount) {
        return WriteLE<uint16_t>(out, methodCount);
    }

    bool WriteMethod(WritableByteBuffer& out, const Method& method) {
        if (!WriteConstantIndex(out, method.name)) return false;
        return WriteConstantIndex(out, method.function);
    }

    bool WriteClassCount(WritableByteBuffer& out, uint16_t classCount) {
        return WriteLE<uint16_t>(out, classCount);
    }

    bool WriteClass(WritableByteBuffer& out, const Class& clas) {
        if (!WriteConstantIndex(out, clas.name)) return false;
        if (!WriteConstantIndex(out, clas.superClass)) return false;
        if (!WriteFieldCount(out, clas.fieldCount)) return false;
        if (!WriteMethodCount(out, clas.methodCount)) return false;

        for (uint16_t i = 0; i < clas.fieldCount; i++) {
            if (!WriteField(out, clas.fields[i])) return false;
        }

        for (uint16_t i = 0; i < clas.methodCount; i++) {
            if (!WriteMethod(out, clas.methods[i])) return false;
        }

        return true;
    }

    bool WriteFunctionCount(WritableByteBuffer& out, uint16_t functionCount) {
        return WriteLE<uint16_t>(out, functionCount);
    }

    bool WriteFunction(WritableByteBuffer& out, const Function& function) {
        if (!WriteConstantIndex(out, function.name)) return false;
        if (!WriteLE<uint16_t>(out, function.flags)) return false;
        if (!WriteLE<uint16_t>(out, function.registerCount)) return false;
        if (!WriteLE<uint16_t>(out, function.parameterCount)) return false;
        if (!WriteLE<uint32_t>(out, function.bytecodeSize)) return false;
        out.write(function.bytecode, function.bytecodeSize);
        return !out.fail();
    }

    bool WriteMagicNumber(WritableByteBuffer& out, uint32_t magicNumber) {
        return WriteLE<uint32_t>(out, magicNumber);
    }

    bool WriteFormatVersion(WritableByteBuffer& out, uint16_t formatVersion) {
        return WriteLE<uint16_t>(out, formatVersion);
    }

    bool WriteModule(WritableByteBuffer& out, const Module& module) {
        if (!WriteMagicNumber(out, module.magic)) return false;
        if (!WriteFormatVersion(out, module.formatVersion)) return false;
        if (!WriteConstantIndex(out, module.name)) return false;
        if (!WriteConstPool(out, module.constPool)) return false;
        if (!WriteClassCount(out, module.classCount)) return false;
        if (!WriteFunctionCount(out, module.functionCount)) return false;

        for (uint16_t i = 0; i < module.classCount; i++) {
            if (!WriteClass(out, module.classes[i])) return false;
        }

        for (uint16_t i = 0; i < module.functionCount; i++) {
            if (!WriteFunction(out, module.functions[i])) return false;
        }

        return true;
    }
}
