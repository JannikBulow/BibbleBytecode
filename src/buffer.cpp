// Copyright 2026 Jannik Laugmand Bülow

#include "BibbleBytecode/buffer.h"

#include <cstring>
#include <fstream>

namespace bibblebytecode {
    ByteBuffer::ByteBuffer(const uint8_t* data, size_t size)
        : mOwner(nullptr)
        , mData(data)
        , mSize(size)
        , mPos(0)
        , mFail(false) {}

    ByteBuffer::ByteBuffer(std::unique_ptr<uint8_t[]> data, size_t size)
        : mOwner(std::move(data))
        , mData(mOwner.get())
        , mSize(size)
        , mPos(0)
        , mFail(false) {}

    void ByteBuffer::seek(size_t newPos) {
        if (mFail) [[unlikely]] return;
        if (newPos > mSize) [[unlikely]] {
            mFail = true;
            return;
        }
        mPos = newPos;
    }

    ByteBuffer& ByteBuffer::read(uint8_t* buf, size_t count) {
        if (mFail) [[unlikely]] return *this;
        if (mPos + count > mSize) [[unlikely]] {
            mFail = true;
            return *this;
        }

        std::memcpy(buf, mData + mPos, count);
        mPos += count;

        return *this;
    }

    ByteBuffer Open(const char* filePath) {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (file.fail()) {
            return {};
        }

        std::streamsize size = file.tellg();
        if (size < 0) {
            return {};
        }

        file.seekg(0, std::ios::beg);

        auto buffer = std::make_unique<uint8_t[]>(size);

        file.read(reinterpret_cast<std::ifstream::char_type*>(buffer.get()), size);

        if (file.fail()) {
            return {};
        }

        return {std::move(buffer), static_cast<size_t>(size)};
    }

    ByteBuffer Open(const std::string& filePath) {
        return Open(filePath.c_str());
    }
}
