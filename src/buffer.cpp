// Copyright 2026 Jannik Laugmand Bülow

#include "BibbleBytecode/buffer.h"

#include <cstring>

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
}
