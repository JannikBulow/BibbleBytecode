// Copyright 2026 Jannik Laugmand Bülow

#include "BibbleBytecode/_compatability.h"
#include "BibbleBytecode/reader.h"

namespace bibblebytecode::reader {
    template<class T>
    std::optional<T> ReadLE(ByteBuffer& in) {
        static_assert(std::is_integral_v<T>, "T must be integral");

        T value;
        in.read(reinterpret_cast<uint8_t*>(&value), sizeof(T));
        if (in.fail()) return std::nullopt;

        // just in case yo
        if constexpr (std::endian::native == std::endian::big) {
            // ReSharper disable once CppDFAUnreachableCode
            if constexpr (sizeof(T) == 2) value = compat::bswap16(value);
            if constexpr (sizeof(T) == 4) value = compat::bswap32(value);
            if constexpr (sizeof(T) == 8) value = compat::bswap64(value);
        }

        return value;
    }

    template<class Len>
    std::optional<std::string_view> ReadString(ByteBuffer& in) {
        std::optional<Len> length = ReadLE<Len>(in);

        const char* data = reinterpret_cast<const char*>(in.raw() + in.tell());
        in.seek(in.tell() + length.value());
        if (in.fail()) return std::nullopt;

        return std::string_view(data, length.value());
    }

    std::optional<uint32_t> ReadMagicNumber(ByteBuffer& in, bool verify) {
        std::optional<uint32_t> value = ReadLE<uint32_t>(in);
        if (!value.has_value()) return std::nullopt;
        if (verify && value.value() != MAGIC) return std::nullopt;
        return value;
    }

    std::optional<uint16_t> ReadFormatVersion(ByteBuffer& in) {
        return ReadLE<uint16_t>(in);
    }

    std::optional<ConstantIndex> ReadConstantIndex(ByteBuffer& in) {
        return ReadLE<ConstantIndex>(in);
    }

    std::optional<ConstPool::Tag> ReadConstantTag(ByteBuffer& in) {
        std::optional<uint8_t> value = ReadLE<uint8_t>(in);
        if (!value.has_value()) return std::nullopt;
        return static_cast<ConstPool::Tag>(value.value());
    }

    std::optional<ConstPool::ModuleInfo> ReadModuleInfo(ByteBuffer& in) {
        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        return ConstPool::ModuleInfo(name.value());
    }

    std::optional<ConstPool::ClassInfo> ReadClassInfo(ByteBuffer& in) {
        std::optional<ConstantIndex> module = ReadConstantIndex(in);
        if (!module.has_value()) return std::nullopt;
        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        return ConstPool::ClassInfo(module.value(), name.value());
    }

    std::optional<ConstPool::FieldInfo> ReadFieldInfo(ByteBuffer& in) {
        std::optional<ConstantIndex> clas = ReadConstantIndex(in);
        if (!clas.has_value()) return std::nullopt;
        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        return ConstPool::FieldInfo(clas.value(), name.value());
    }

    std::optional<ConstPool::MethodInfo> ReadMethodInfo(ByteBuffer& in) {
        std::optional<ConstantIndex> clas = ReadConstantIndex(in);
        if (!clas.has_value()) return std::nullopt;
        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        return ConstPool::MethodInfo(clas.value(), name.value());
    }

    std::optional<ConstPool::FunctionInfo> ReadFunctionInfo(ByteBuffer& in) {
        std::optional<ConstantIndex> module = ReadConstantIndex(in);
        if (!module.has_value()) return std::nullopt;
        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        return ConstPool::FunctionInfo(module.value(), name.value());
    }

    std::optional<ConstPool::Entry> ReadConstantEntry(ByteBuffer& in, ConstPool::Tag tag) {
        ConstPool::Entry entry{};
        entry.tag = tag;

        switch (tag) {
            case ConstPool::BYTE: {
                std::optional<uint8_t> value = ReadLE<uint8_t>(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.b = static_cast<int8_t>(value.value());
                break;
            }
            case ConstPool::SHORT: {
                std::optional<uint16_t> value = ReadLE<uint16_t>(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.s = static_cast<int16_t>(value.value());
                break;
            }
            case ConstPool::INT: {
                std::optional<uint32_t> value = ReadLE<uint32_t>(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.i = static_cast<int32_t>(value.value());
                break;
            }
            case ConstPool::LONG: {
                std::optional<uint64_t> value = ReadLE<uint64_t>(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.l = static_cast<int64_t>(value.value());
                break;
            }
            case ConstPool::STRING: {
                std::optional<std::string_view> value = ReadString<uint32_t>(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.str = value.value();
                break;
            }
            case ConstPool::MODULE_INFO: {
                std::optional<ConstPool::ModuleInfo> value = ReadModuleInfo(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.mi = value.value();
                break;
            }
            case ConstPool::CLASS_INFO: {
                std::optional<ConstPool::ClassInfo> value = ReadClassInfo(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.ci = value.value();
                break;
            }
            case ConstPool::FIELD_INFO: {
                std::optional<ConstPool::FieldInfo> value = ReadFieldInfo(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.fi = value.value();
                break;
            }
            case ConstPool::METHOD_INFO: {
                std::optional<ConstPool::MethodInfo> value = ReadMethodInfo(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.mei = value.value();
                break;
            }
            case ConstPool::FUNCTION_INFO: {
                std::optional<ConstPool::FunctionInfo> value = ReadFunctionInfo(in);
                if (!value.has_value()) return std::nullopt;
                entry.u.fni = value.value();
                break;
            }
        }

        return entry;
    }

    std::optional<ConstPool::Entry> ReadConstantTagAndEntry(ByteBuffer& in) {
        std::optional<ConstPool::Tag> tag = ReadConstantTag(in);
        if (!tag.has_value()) return std::nullopt;
        return ReadConstantEntry(in, tag.value());
    }

    std::optional<uint16_t> ReadConstantEntryCount(ByteBuffer& in) {
        return ReadLE<uint16_t>(in);
    }

    std::optional<ConstPool> ReadConstPool(ByteBuffer& in, const AllocatorView& allocator) {
        std::optional<uint16_t> entryCount = ReadConstantEntryCount(in);
        if (!entryCount.has_value()) return std::nullopt;

        ConstPool::Entry* entries = static_cast<ConstPool::Entry*>(allocator.allocate(entryCount.value() * sizeof(ConstPool::Entry)));
        if (entries == nullptr) return std::nullopt;

        for (uint16_t i = 1; i < entryCount.value(); ++i) {
            std::optional<ConstPool::Entry> entry = ReadConstantTagAndEntry(in);
            if (!entry.has_value()) {
                allocator.deallocate(entries);
                return std::nullopt;
            }
            entries[i] = entry.value();
        }

        return ConstPool(entryCount.value(), entries);
    }

    std::optional<uint16_t> ReadFieldCount(ByteBuffer& in) {
        return ReadLE<uint16_t>(in);
    }

    std::optional<Field> ReadField(ByteBuffer& in) {
        std::optional<uint32_t> typeID = ReadLE<uint32_t>(in);
        if (!typeID.has_value()) return std::nullopt;

        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        return Field(typeID.value(), name.value());
    }

    std::optional<uint16_t> ReadMethodCount(ByteBuffer& in) {
        return ReadLE<uint16_t>(in);
    }

    std::optional<Method> ReadMethod(ByteBuffer& in) {
        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        std::optional<ConstantIndex> function = ReadConstantIndex(in);
        if (!function.has_value()) return std::nullopt;

        return Method(name.value(), function.value());
    }

    std::optional<uint16_t> ReadClassCount(ByteBuffer& in) {
        return ReadLE<uint16_t>(in);
    }

    std::optional<Class> ReadClass(ByteBuffer& in, const AllocatorView& allocator) {
        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        std::optional<ConstantIndex> superClass = ReadConstantIndex(in);
        if (!superClass.has_value()) return std::nullopt;

        std::optional<uint16_t> fieldCount = ReadLE<uint16_t>(in);
        if (!fieldCount.has_value()) return std::nullopt;

        std::optional<uint16_t> methodCount = ReadLE<uint16_t>(in);
        if (!methodCount.has_value()) return std::nullopt;

        Field* fields = static_cast<Field*>(allocator.allocate(fieldCount.value() * sizeof(Field)));
        if (fields == nullptr) return std::nullopt;

        for (uint16_t i = 0; i < fieldCount.value(); i++) {
            std::optional<Field> field = ReadField(in);
            if (!field.has_value()) {
                allocator.deallocate(fields);
                return std::nullopt;
            }
            fields[i] = field.value();
        }

        Method* methods = static_cast<Method*>(allocator.allocate(methodCount.value() * sizeof(Method)));
        if (methods == nullptr) {
            allocator.deallocate(fields);
            return std::nullopt;
        }

        for (uint16_t i = 0; i < methodCount.value(); i++) {
            std::optional<Method> method = ReadMethod(in);
            if (!method.has_value()) {
                allocator.deallocate(fields);
                allocator.deallocate(methods);
                return std::nullopt;
            }
            methods[i] = method.value();
        }
    }

    std::optional<uint16_t> ReadFunctionCount(ByteBuffer& in) {
        return ReadLE<uint16_t>(in);
    }

    std::optional<Function> ReadFunction(ByteBuffer& in) {
        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        std::optional<uint16_t> flags = ReadLE<uint16_t>(in);
        if (!flags.has_value()) return std::nullopt;

        std::optional<uint16_t> registerCount = ReadLE<uint16_t>(in);
        if (!registerCount.has_value()) return std::nullopt;

        std::optional<uint16_t> parameterCount = ReadLE<uint16_t>(in);
        if (!parameterCount.has_value()) return std::nullopt;

        std::optional<uint32_t> bytecodeSize = ReadLE<uint32_t>(in);
        if (!bytecodeSize.has_value()) return std::nullopt;

        const uint8_t* bytecode = in.raw() + in.tell();
        in.seek(in.tell() + bytecodeSize.value());
        if (in.fail()) return std::nullopt;

        return Function(name.value(), flags.value(), registerCount.value(), parameterCount.value(), bytecodeSize.value(), bytecode);
    }

    std::optional<Module> ReadModule(ByteBuffer& in, const AllocatorView& allocator) {
        std::optional<uint32_t> magic = ReadMagicNumber(in, true);
        if (!magic.has_value()) return std::nullopt;

        std::optional<uint16_t> formatVersion = ReadFormatVersion(in);
        if (!formatVersion.has_value()) return std::nullopt; //TODO: read based on format version if the file format ever changes

        std::optional<ConstantIndex> name = ReadConstantIndex(in);
        if (!name.has_value()) return std::nullopt;

        std::optional<ConstPool> constPool = ReadConstPool(in, allocator);
        if (!constPool.has_value()) return std::nullopt;

        std::optional<uint16_t> classCount = ReadClassCount(in);
        if (!classCount.has_value()) return std::nullopt;

        std::optional<uint16_t> functionCount = ReadFunctionCount(in);
        if (!functionCount.has_value()) return std::nullopt;

        Class* classes = static_cast<Class*>(allocator.allocate(classCount.value() * sizeof(Class)));
        if (classes == nullptr) return std::nullopt;

        for (uint16_t i = 0; i < classCount.value(); i++) {
            std::optional<Class> clas = ReadClass(in, allocator);
            if (!clas.has_value()) {
                allocator.deallocate(classes);
                return std::nullopt;
            }
            classes[i] = clas.value();
        }

        Function* functions = static_cast<Function*>(allocator.allocate(functionCount.value() * sizeof(Function)));
        if (functions == nullptr) {
            allocator.deallocate(classes);
            return std::nullopt;
        }

        for (uint16_t i = 0; i < functionCount.value(); i++) {
            std::optional<Function> function = ReadFunction(in);
            if (!function.has_value()) {
                allocator.deallocate(classes);
                allocator.deallocate(functions);
                return std::nullopt;
            }
            functions[i] = function.value();
        }

        return Module(magic.value(), formatVersion.value(), name.value(), constPool.value(), classCount.value(), functionCount.value(), classes, functions);
    }
}
