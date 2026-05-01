// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_BUFFER_H
#define BIBBLEBYTECODE_BUFFER_H 1

#include "BibbleBytecode/api.h"

#include <cstdint>
#include <memory>
#include <string>

namespace bibblebytecode {
    class BIBBLEBYTECODE_EXPORT ReadableByteBuffer {
    public:
        ReadableByteBuffer()
            : mOwner(nullptr)
            , mData(nullptr)
            , mSize(0)
            , mPos(0)
            , mFail(true) {}

        ReadableByteBuffer(const uint8_t* data, size_t size);
        ReadableByteBuffer(std::unique_ptr<uint8_t[]> data, size_t size);

        bool fail() const { return mFail; }

        const uint8_t* raw() const { return mData; }

        void seek(size_t newPos);
        size_t tell() const { return mPos; }

        ReadableByteBuffer& read(uint8_t* buf, size_t count);

    private:
        std::unique_ptr<uint8_t[]> mOwner;
        const uint8_t* mData;
        size_t mSize;
        size_t mPos;
        bool mFail;
    };

    BIBBLEBYTECODE_EXPORT ReadableByteBuffer Open(const char* filePath);
    BIBBLEBYTECODE_EXPORT ReadableByteBuffer Open(const std::string& filePath);
}

#endif //BIBBLEBYTECODE_BUFFER_H