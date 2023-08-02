#pragma once

#include "varia/VShared.hpp"

template <Integer_64 SIZE>
struct String_Buffer
{
    Integer_64 write_head;
    char data[SIZE];
};

struct String_Buffer_Reference
{
    char const * string;
    Integer_64 length;
};

template <Integer_64 SIZE>
String_Buffer_Reference v_string_buffer_emplace_string(String_Buffer<SIZE> * sb, char const * string)
{
    Integer_64 const limit = 100000;
    Integer_64 string_length = strnlen(string, limit);

    Integer_64 safety_check = (string_length > limit) ? limit : string_length;
    safety_check += 1; //Null terminator

    VARIA_ASSERT( (sb->write_head + safety_check) < SIZE, "String buffer overrun");

    String_Buffer_Reference sbr = ZERO_INIT();
    if (sb->write_head + safety_check < SIZE)
    {
        char * string_write_location = address_of(sb->data[sb->write_head]);
        memcpy(string_write_location, string, string_length);

        sb->write_head += string_length + 1;
        sb->data[sb->write_head] = '\0';
        sb->write_head += 1;

        sbr.string = string_write_location;
        sbr.length = string_length;
    }
    else
    {
        sbr.string = "String buffer storage was overrun and your string was not emplaced. This stub was returned.";
        sbr.length = 0;
    }

    return sbr;
}

template <Integer_64 SIZE>
void v_string_buffer_clear(String_Buffer<SIZE> * sb)
{
    sb->write_head = 0;
}
