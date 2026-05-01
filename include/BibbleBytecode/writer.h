// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_WRITER_H
#define BIBBLEBYTECODE_WRITER_H 1

#include "BibbleBytecode/module/module.h"

#include "BibbleBytecode/buffer.h"

#include <optional>

#include "BibbleBytecode/module/const_pool.h"

// Every small part of the module is individually readable so freaks can do it their own special ways if desired
namespace bibblebytecode::writer {
    BIBBLEBYTECODE_EXPORT bool WriteConstantIndex(WritableByteBuffer& out, ConstantIndex index); // sorry twin, but i'm not making one of these for every single type of constant index

    BIBBLEBYTECODE_EXPORT bool WriteConstantTag(WritableByteBuffer& out, ConstPool::Tag tag);
    BIBBLEBYTECODE_EXPORT bool WriteModuleInfo(WritableByteBuffer& out, const ConstPool::ModuleInfo& moduleInfo);
    BIBBLEBYTECODE_EXPORT bool WriteClassInfo(WritableByteBuffer& out, const ConstPool::ClassInfo& classInfo);
    BIBBLEBYTECODE_EXPORT bool WriteFieldInfo(WritableByteBuffer& out, const ConstPool::FieldInfo& fieldInfo);
    BIBBLEBYTECODE_EXPORT bool WriteMethodInfo(WritableByteBuffer& out, const ConstPool::MethodInfo& methodInfo);
    BIBBLEBYTECODE_EXPORT bool WriteFunctionInfo(WritableByteBuffer& out, const ConstPool::FunctionInfo& functionInfo);
    BIBBLEBYTECODE_EXPORT bool WriteConstantEntry(WritableByteBuffer& out, const ConstPool::Entry& entry); // this expects tag being written already
    BIBBLEBYTECODE_EXPORT bool WriteConstantTagAndEntry(WritableByteBuffer& out, const ConstPool::Entry& entry); // this will write tag unlike the one above
    BIBBLEBYTECODE_EXPORT bool WriteConstantEntryCount(WritableByteBuffer& out, uint16_t entryCount);
    BIBBLEBYTECODE_EXPORT bool WriteConstPool(WritableByteBuffer& out, const ConstPool& pool);

    BIBBLEBYTECODE_EXPORT bool WriteFieldCount(WritableByteBuffer& out, uint16_t fieldCount);
    BIBBLEBYTECODE_EXPORT bool WriteField(WritableByteBuffer& out, const Field& field);

    BIBBLEBYTECODE_EXPORT bool WriteMethodCount(WritableByteBuffer& out, uint16_t methodCount);
    BIBBLEBYTECODE_EXPORT bool WriteMethod(WritableByteBuffer& out, const Method& method);

    BIBBLEBYTECODE_EXPORT bool WriteClassCount(WritableByteBuffer& out, uint16_t classCount);
    BIBBLEBYTECODE_EXPORT bool WriteClass(WritableByteBuffer& out, const Class& clas);

    BIBBLEBYTECODE_EXPORT bool WriteFunctionCount(WritableByteBuffer& out, uint16_t functionCount);
    BIBBLEBYTECODE_EXPORT bool WriteFunction(WritableByteBuffer& out, const Function& function);

    BIBBLEBYTECODE_EXPORT bool WriteMagicNumber(WritableByteBuffer& out, uint32_t magicNumber);
    BIBBLEBYTECODE_EXPORT bool WriteFormatVersion(WritableByteBuffer& out, uint16_t formatVersion);

    // Just use this, man
    BIBBLEBYTECODE_EXPORT bool WriteModule(WritableByteBuffer& out, const Module& module);
}

#endif //BIBBLEBYTECODE_WRITER_H