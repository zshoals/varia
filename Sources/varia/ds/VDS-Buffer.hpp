#pragma once

#include "VDS-Types.hpp"

template <Integer_64 SIZE>
struct VDS_Buffer
{
    Byte data[SIZE];
};

template <Integer_64 SIZE>
constexpr Integer_64 vds_buffer_capacity(VDS_Buffer<SIZE> const * buffer)
{
    return SIZE;
}