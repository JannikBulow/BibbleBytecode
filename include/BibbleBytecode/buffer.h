// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_BUFFER_H
#define BIBBLEBYTECODE_BUFFER_H 1

#include <cstdint>

namespace bibblebytecode {
    class ByteBuffer {
    public:
        ByteBuffer(const uint8_t* data, size_t size);

        bool fail() const { return mFail; }

        const uint8_t* raw() const { return mData; }

        void seek(size_t newPos);
        size_t tell() const { return mPos; }

        ByteBuffer& read(uint8_t* buf, size_t count);

    private:
        const uint8_t* mData;
        size_t mSize;
        size_t mPos;
        bool mFail;
    };
}

#endif //BIBBLEBYTECODE_BUFFER_H