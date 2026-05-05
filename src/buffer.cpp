// Copyright 2026 Jannik Laugmand Bülow

#include "BibbleBytecode/buffer.h"

#include <cstring>
#include <fstream>

namespace bibblebytecode {
    ReadableByteBuffer::ReadableByteBuffer(const uint8_t* data, size_t size)
        : mOwner(nullptr)
        , mData(data)
        , mSize(size)
        , mPos(0)
        , mFail(false) {}

    ReadableByteBuffer::ReadableByteBuffer(std::unique_ptr<uint8_t[]> data, size_t size)
        : mOwner(std::move(data))
        , mData(mOwner.get())
        , mSize(size)
        , mPos(0)
        , mFail(false) {}

    void ReadableByteBuffer::seek(size_t newPos) {
        if (mFail) [[unlikely]] return;
        if (newPos > mSize) [[unlikely]] {
            mFail = true;
            return;
        }
        mPos = newPos;
    }

    ReadableByteBuffer& ReadableByteBuffer::read(uint8_t* buf, size_t count) {
        if (mFail) [[unlikely]] return *this;
        if (mPos + count > mSize) [[unlikely]] {
            mFail = true;
            return *this;
        }

        std::memcpy(buf, mData + mPos, count);
        mPos += count;

        return *this;
    }

    WritableByteBuffer::WritableByteBuffer()
        : mData(nullptr)
        , mSize(0)
        , mCapacity(0)
        , mPos(0)
        , mFail(false) {}

    void WritableByteBuffer::seek(size_t newPos) {
        if (mFail) [[unlikely]] return;
        if (newPos > mSize) [[unlikely]] {
            mFail = true;
            return;
        }
        mPos = newPos;
    }

    void WritableByteBuffer::reserve(size_t capacity) {
        if (mFail) [[unlikely]] return;
        if (capacity <= mCapacity) return;

        std::unique_ptr<uint8_t[]> newBuffer = std::make_unique<uint8_t[]>(capacity);
        if (mData != nullptr) std::memcpy(newBuffer.get(), mData.get(), mCapacity);

        mData = std::move(newBuffer);
        mCapacity = capacity;
    }

    void WritableByteBuffer::ensureCapacity(size_t neededBytes) {
        if (mFail) [[unlikely]] return;
        if (mPos + neededBytes <= mCapacity) return;

        size_t newCapacity = mCapacity != 0 ? mCapacity * 2 : 512;
        if (newCapacity < mPos + neededBytes) newCapacity = mPos + neededBytes;

        reserve(newCapacity);
    }

    WritableByteBuffer& WritableByteBuffer::write(const uint8_t* data, size_t count) {
        if (mFail) [[unlikely]] return *this;
        ensureCapacity(count);

        if (mPos > mSize) [[unlikely]] {
            std::memset(mData.get() + mSize, 0, mPos - mSize);
        }

        std::memcpy(mData.get() + mPos, data, count);

        mPos += count;
        mSize = std::max(mSize, mPos);

        return *this;
    }

    void WritableByteBuffer::emit(std::ostream& out) const {
        if (mFail) [[unlikely]] return;
        out.write(reinterpret_cast<const std::ostream::char_type*>(mData.get()), static_cast<std::streamsize>(mSize));
    }

    ReadableByteBuffer Open(const char* filePath) {
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

    ReadableByteBuffer Open(const std::string& filePath) {
        return Open(filePath.c_str());
    }
}
