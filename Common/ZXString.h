#pragma once
#include "logger.h"

// good reference: https://en.cppreference.com/w/cpp/language/operators

template <typename T>
struct ZXString
{
    T* m_pStr;

    ZXString()
    {
        m_pStr = 0;
    }
    ZXString(T* val)
    {
        m_pStr = &val;
    }
};

assert_size(sizeof(ZXString<char>), 0x04)