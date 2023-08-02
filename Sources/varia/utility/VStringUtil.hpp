#pragma once

#include "varia/VShared.hpp"

#include <string.h>

static inline Integer_64 v_string_utility_char_match(char const * left, char const * right)
{
    return ( (*left) == (*right) );
}

static inline Integer_64 v_string_utility_length(char const * string)
{
    return strnlen(string, 100000);
}

static inline Boolean v_string_utility_is_numeric(char c)
{
    return ((c >= '0') && (c <= '9'));
}

static inline Boolean v_string_utility_is_carriage_return(char c)
{
    return (c == '\r');
}

static inline Boolean v_string_utility_is_newline(char c)
{
    return (c == '\n');
}

static inline Boolean v_string_utility_is_combined_newline(char const * c)
{
    Boolean has_carriage_return = v_string_utility_is_carriage_return(*c);
    Boolean has_newline = v_string_utility_is_newline(*c);
    Boolean has_newline_2 = v_string_utility_is_newline( *(c + 1) );
    
    if (has_newline)
    {
        return true;
    }

    if (has_carriage_return & has_newline_2)
    {
        return true;
    }

    return false;
}

static inline Integer_32 v_string_utility_count_newline_characters(char const * c)
{
    Boolean has_carriage_return = v_string_utility_is_carriage_return(*c);
    Boolean has_newline = v_string_utility_is_newline(*c);
    Boolean has_newline_2 = v_string_utility_is_newline( *(c + 1) );

    if (has_newline)
    {
        return 1;
    }

    if (has_carriage_return & has_newline_2)
    {
        return 2;
    }

    return 0;
}