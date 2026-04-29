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
    BIBBLEBYTECODE_EXPORT std::optional<ConstantIndex> ReadConstantIndex(ByteBuffer& in); // sorry twin, but i'm not making one of these for every single type of constant index

    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::Tag> ReadConstantTag(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::ModuleInfo> ReadModuleInfo(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::ClassInfo> ReadClassInfo(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::FieldInfo> ReadFieldInfo(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::MethodInfo> ReadMethodInfo(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::FunctionInfo> ReadFunctionInfo(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::Entry> ReadConstantEntry(ByteBuffer& in, ConstPool::Tag tag); // this expects tag being read already
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool::Entry> ReadConstantTagAndEntry(ByteBuffer& in); // this will read tag unlike the one above
    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadConstantEntryCount(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<ConstPool> ReadConstPool(ByteBuffer& in, const AllocatorView& allocator);

    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadFieldCount(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<Field> ReadField(ByteBuffer& in);

    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadMethodCount(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<Method> ReadMethod(ByteBuffer& in);

    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadClassCount(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<Class> ReadClass(ByteBuffer& in, const AllocatorView& allocator);

    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadFunctionCount(ByteBuffer& in);
    BIBBLEBYTECODE_EXPORT std::optional<Function> ReadFunction(ByteBuffer& in);

    BIBBLEBYTECODE_EXPORT std::optional<uint32_t> ReadMagicNumber(ByteBuffer& in, bool verify = true);
    BIBBLEBYTECODE_EXPORT std::optional<uint16_t> ReadFormatVersion(ByteBuffer& in);

    // Just use this, man
    BIBBLEBYTECODE_EXPORT std::optional<Module> ReadModule(ByteBuffer& in, const AllocatorView& allocator);
}

#endif //BIBBLEBYTECODE_READER_H