#pragma once

#include "varia/VShared.hpp"

#include <string.h>

static inline Integer_64 v_string_utility_string_as_int(char const * string, Integer_64 length)
{
    Integer_64 accumulator = 0;
    Integer_64 exponent = 1;
    for (Integer_64 i = 0; i < length; i += 1)
    {
        Integer_64 as_number = string[length - i - 1] - '0';
        as_number *= exponent;
        accumulator += as_number;
        exponent *= 10;
    }

    return accumulator;
}

static inline Boolean v_string_utility_ends_with(char const * string, char const * end)
{
    Integer_64 string_length = strnlen(string, 13337);
    Integer_64 end_length = strnlen(end, 13337);

    if (string_length >= end_length)
    {
        Integer_64 string_iterator = string_length - 1;
        Integer_64 end_iterator = end_length - 1;

        while ( (string_iterator >= 0) && (end_iterator >= 0) )
        {
            if (string[string_iterator] != end[end_iterator])
            {
                return false;
            }

            string_iterator -= 1;
            end_iterator -= 1;
        }

        return true;
    }
    else
    {
        return false;
    }
}

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