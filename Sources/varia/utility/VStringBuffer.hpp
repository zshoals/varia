#pragma once

#include "varia/VShared.hpp"
#include "varia/utility/VMemcpy.hpp"
#include "kinc/log.h"

template <Integer_64 SIZE>
struct String_Buffer_Storage
{
    Integer_64 write_head;
    char data[SIZE];
};

struct String_Buffer
{
    Integer_64 capacity;
    Integer_64 * write_head;
    char * data;
};

struct String_Buffer_Reference
{
    char const * string;
    Integer_64 length;
};

template <Integer_64 SIZE>
String_Buffer v_string_buffer_make_interface(String_Buffer_Storage<SIZE> * sb)
{
    String_Buffer interface;
    {
        interface.capacity = SIZE;
        interface.write_head = &(sb->write_head);
        interface.data = &(sb->data[0]);
    }

    return interface;
}

static inline String_Buffer_Reference v_string_buffer_emplace_string(String_Buffer * sb, char const * string, Integer_64 string_length)
{
    Integer_64 const limit = 100000;

    Integer_64 safety_check = (string_length > limit) ? limit : string_length;
    safety_check += 1; //Null terminator

    VARIA_ASSERT( ( *(sb->write_head) + safety_check) < sb->capacity, "String buffer overrun");

    String_Buffer_Reference sbr = ZERO_INIT();
    if ( *(sb->write_head) + safety_check < sb->capacity)
    {
        char * string_write_location = address_of(sb->data[*(sb->write_head)]);
        memcpy(string_write_location, string, string_length);

        *(sb->write_head) += string_length + 1;
        sb->data[*(sb->write_head)] = '\0';
        *(sb->write_head) += 1;

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

static inline void v_string_buffer_clear(String_Buffer * sb)
{
    *(sb->write_head) = 0;
}

static inline void v_string_buffer_print_reference(String_Buffer_Reference const & ref)
{
    kinc_log(KINC_LOG_LEVEL_INFO, "SBR: %s Length: %ld", ref.string, ref.length);
}