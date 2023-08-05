#pragma once

#include "VDS-Types.hpp"
#include "VDS-Util.hpp"
#include "VDS-Arena.hpp"

#include <string.h>

struct VDS_String
{
    char const * string;
    Integer_64 length;
};

static inline VDS_String vds_string_emplace_string_count(VDS_Arena * arena, char const * literal, Integer_64 length)
{
    void * memory = vds_arena_allocate(arena, length + 1);
    char const * strbase = static_cast<char const *>(memory);
    strbase[length + 1] = '\0';

    VDS_String string;
    {
        string.string = strbase;
        string.length = length;
    }

    return string;
}

static inline VDS_String vds_string_emplace_string(VDS_Arena * arena, char const * literal)
{
    Integer_64 length = strlen(literal);
    return vds_string_emplace_string_count(arena, literal, length);
}

static inline Boolean vds_string_matches(VDS_String const & left, VDS_String const & right)
{
    if (left.length != right.length) { return false; }

    return (strcmp(&(left.string), &(right.string)) == 0);
}