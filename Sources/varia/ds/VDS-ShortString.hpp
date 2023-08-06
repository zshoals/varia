#pragma once

#include "VDS-Types.hpp"
#include "VDS-Util.hpp"

#include <string.h>

#define VDS_SHORT_STRING_CAPACITY 63
struct VDS_Short_String
{
    char length;
    char string[VDS_SHORT_STRING_CAPACITY];
};

static inline VDS_Short_String vds_short_string_from_literal_count(char const * literal, Integer_64 length)
{
    constexpr char capacity = VDS_SHORT_STRING_CAPACITY - 1;
    if (length > capacity) { length = capacity; }
    if (length < 0) { length = 0; }

    VDS_Short_String ss = {};
    {
        memcpy(&(ss.string[0]), literal, length);
        ss.length = (char)length;
        ss.string[length] = '\0';
        ss.string[capacity] = '\0';
    }

    return ss;
}

static inline VDS_Short_String vds_short_string_from_literal(char const * literal)
{
    Integer_64 length = strlen(literal); 
    return vds_short_string_from_literal_count(literal, length);
}

static inline Boolean vds_short_string_matches(VDS_Short_String const & left, VDS_Short_String const & right)
{
    if (left.length != right.length) { return false; }
    return (strncmp(&(left.string[0]), &(right.string[0]), left.length) == 0);
}