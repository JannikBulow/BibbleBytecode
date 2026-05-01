// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_READER_H
#define BIBBLEBYTECODE_READER_H 1

#include "BibbleBytecode/module/module.h"

#include "BibbleBytecode/allocator_view.h"
#include "BibbleBytecode/api.h"
#include "BibbleBytecode/buffer.h"

#include <optional>

// Every small part of the module is individually readable so freaks can do it their own special ways if desired
namespace bibblebytecode::reader {
    BIBBLEBYTECODE_EXPORT std::optional<ConstantIndex> ReadConstantIndex(ReadableByteBuffer& in); // sorry twin, but i'm not making one of these for every single type of constant index

    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::Tag> ReadConstantTag(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::ModuleInfo> ReadModuleInfo(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::ClassInfo> ReadClassInfo(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::FieldInfo> ReadFieldInfo(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::MethodInfo> ReadMethodInfo(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::FunctionInfo> ReadFunctionInfo(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::Entry> ReadConstantEntry(ReadableByteBuffer& in, ConstPool::Tag tag); // this expects tag being read already
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::Entry> ReadConstantTagAndEntry(ReadableByteBuffer& in); // this will read tag unlike the one above
    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadConstantEntryCount(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool> ReadConstPool(ReadableByteBuffer& in, const AllocatorView& allocator);

    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadFieldCount(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<Field> ReadField(ReadableByteBuffer& in);

    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadMethodCount(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<Method> ReadMethod(ReadableByteBuffer& in);

    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadClassCount(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<Class> ReadClass(ReadableByteBuffer& in, const AllocatorView& allocator);

    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadFunctionCount(ReadableByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<Function> ReadFunction(ReadableByteBuffer& in);

    BIBBLEBYTECODE_EXPORT std::optional<uint32_t> ReadMagicNumber(ReadableByteBuffer& in, bool verify = true);
    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadFormatVersion(ReadableByteBuffer& in);

    // Just use this, man
    BIBBLEBYTECODE_EXPORT std::optional<Module> ReadModule(ReadableByteBuffer& in, const AllocatorView& allocator);
}

#endif //BIBBLEBYTECODE_READER_H