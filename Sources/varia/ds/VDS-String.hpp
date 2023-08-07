#pragma once

#include "VDS-Types.hpp"
#include "VDS-Util.hpp"

#include <string.h>

template <Integer_32 SIZE>
struct VDS_String
{
    Integer_32 length;
    char string[SIZE];

    static constexpr Integer_32 capacity = SIZE - sizeof(length) - 1;

    VDS_String()
    {
        memset(this, 0, sizeof(*this));
    }

    VDS_String(char const * string)
    {
        memset(this, 0, sizeof(*this));
        this->length = (Integer_32)strnlen(string, this->capacity);
        memcpy(&(this->string[0]), string, this->length);
        this->string[this->length] = '\0';
        this->string[this->capacity] = '\0';
    }
};

template <Integer_32 SIZE_A, Integer_32 SIZE_B>
Boolean vds_string_matches(VDS_String<SIZE_A> const & left, VDS_String<SIZE_B> const & right)
{
    if (left.length != right.length) { return false; }
    Integer_32 least = (left.length > right.length) ? right.length : left.length;

    return (strncmp(&(left.string[0]), &(right.string[0]), least) == 0);
}

template <Integer_32 SIZE_A, Integer_32 SIZE_B>
VDS_String<SIZE_A> vds_string_append(VDS_String<SIZE_A> const & source, VDS_String<SIZE_B> const & other)
{
        VDS_String<SIZE_A> out = {};

        int source_write_len = (source.length > source.capacity) ? source.capacity : source.length;
        memcpy(&(out.string[0]), &(source.string[0]), source_write_len);

        int remaining_capacity = source.capacity - source_write_len;
        int remaining_write_len = (other.length > remaining_capacity) ? remaining_capacity : other.length;
        memcpy(&(out.string[source_write_len]), &(other.string[0]), remaining_write_len);

        out.length = source_write_len + remaining_write_len;
        out.string[out.length] = '\0';

        return out;
}

template <Integer_32 SIZE_A, Integer_32 SIZE_B>
void vds_string_append_in_place(VDS_String<SIZE_A> & source, VDS_String<SIZE_B> const & other)
{
        int remaining_capacity = source.capacity - source.length;
        int to_write = (other.length > remaining_capacity) ? remaining_capacity : other.length;
        memcpy(&(source.string[source.length]), &(other.string[0]), to_write);

        source.length += to_write;
        source.string[source.length] = '\0';
}

template <Integer_32 SIZE>
char const * vds_string_cstr(VDS_String<SIZE> const & s)
{
    return &(s.string[0]);
}

template <Integer_32 SIZE>
Integer_32 vds_string_length(VDS_String<SIZE> const & s)
{
    return s.length;
}