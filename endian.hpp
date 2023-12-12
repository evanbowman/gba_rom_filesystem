////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2023  Evan Bowman.
//
// The source code in this file is public domain.
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include <string.h>
#include <bit>
#include <stdint.h>


// Because most processors are little endian, I am using little endian byte
// order for binary encoded data.


#ifdef __bswap_constant_16
#undef __bswap_constant_16
#endif
#define __bswap_constant_16(x) ((uint16_t)((((x) >> 8) & 0xff) | (((x)&0xff) << 8)))

#ifdef __bswap_constant_32
#undef __bswap_constant_32
#endif
#define __bswap_constant_32(x)                                                 \
    ((((x)&0xff000000) >> 24) | (((x)&0x00ff0000) >> 8) |                      \
     (((x)&0x0000ff00) << 8) | (((x)&0x000000ff) << 24))

#ifdef __bswap_constant_64
#undef __bswap_constant_64
#endif
#define __bswap_constant_64(x)                                                 \
    ((((x)&0xff00000000000000ull) >> 56) |                                     \
     (((x)&0x00ff000000000000ull) >> 40) |                                     \
     (((x)&0x0000ff0000000000ull) >> 24) |                                     \
     (((x)&0x000000ff00000000ull) >> 8) | (((x)&0x00000000ff000000ull) << 8) | \
     (((x)&0x0000000000ff0000ull) << 24) |                                     \
     (((x)&0x000000000000ff00ull) << 40) |                                     \
     (((x)&0x00000000000000ffull) << 56))


// Added when publicizing the library. Assume gba.
#define __GBA__


#if defined(__GBA__) or defined(__NDS__)
inline constexpr bool is_little_endian()
{
    return true;
}
#else
inline bool is_little_endian()
{
    // FIXME: use std::endian
    static const uint16_t i = 0x000f;
    return ((unsigned char*)&i)[0] == 0x0f;
}
#endif



template <typename T> T to_host_order(T value);


template <> inline unsigned char to_host_order(unsigned char val)
{
    return val;
}

template <> inline uint16_t to_host_order(uint16_t val)
{
    if (is_little_endian()) {
        return val;
    } else {
        return __bswap_constant_16(val);
    }
}

template <> inline int16_t to_host_order(int16_t val)
{
    if (is_little_endian()) {
        return val;
    } else {
        return __bswap_constant_16(val);
    }
}

template <> inline uint32_t to_host_order(uint32_t val)
{
    if (is_little_endian()) {
        return val;
    } else {
        return __bswap_constant_32(val);
    }
}

template <> inline int32_t to_host_order(int32_t val)
{
    if (is_little_endian()) {
        return val;
    } else {
        return __bswap_constant_32(val);
    }
}

template <> inline uint64_t to_host_order(uint64_t val)
{
    if (is_little_endian()) {
        return val;
    } else {
        return __bswap_constant_64(val);
    }
}

template <> inline int64_t to_host_order(int64_t val)
{
    if (is_little_endian()) {
        return val;
    } else {
        return __bswap_constant_64(val);
    }
}


template <typename T> class HostInteger
{
public:
    // NOTE: I could overload the cast operator and assignment operators to make
    // this class behave similarly to a plain integer. But then I get
    // class-memaccess errors, which I don't want to disable throughout the
    // project... calling get()/set() isn't so bad.

    HostInteger() = default;

    explicit HostInteger(T value)
    {
        set(value);
    }

    void set(T value)
    {
        value = to_host_order(value);
        memcpy(data_, &value, sizeof(T));
    }

    T get() const
    {
        T value;
        memcpy(&value, data_, sizeof(T));
        return to_host_order(value);
    }

private:
    char data_[sizeof(T)];
};


using host_u16 = HostInteger<uint16_t>;
using host_u32 = HostInteger<uint32_t>;
using host_s16 = HostInteger<int16_t>;
using host_s32 = HostInteger<int32_t>;
using host_u64 = HostInteger<uint64_t>;
using host_s64 = HostInteger<int64_t>;
