#pragma once

#include "varia/VShared.hpp"
#include "varia/utility/VMemcpy.hpp"
#include "kinc/log.h"

template <Integer_64 SIZE>
struct VDS_String_Buffer
{
    Integer_64 write_head;
    char data[SIZE];
};

struct VDS_String_Buffer_Reference
{
    char const * string;
    Integer_64 length;
};

template <Integer_64 SIZE>
VDS_String_Buffer_Reference vds_string_buffer_emplace_string(VDS_String_Buffer<SIZE> * sb, char const * string, Integer_64 string_length)
{
    Integer_64 const limit = 100000;

    Integer_64 safety_check = (string_length > limit) ? limit : string_length;
    safety_check += 1; //Null terminator

    VARIA_ASSERT( ( sb->write_head + safety_check) < SIZE, "String buffer overrun");

    VDS_String_Buffer_Reference sbr = ZERO_INIT();
    if ( sb->write_head + safety_check < SIZE)
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
void vds_string_buffer_clear(VDS_String_Buffer<SIZE> * sb)
{
    sb->write_head = 0;
}

static inline void v_string_buffer_print_reference(VDS_String_Buffer_Reference const & ref)
{
    kinc_log(KINC_LOG_LEVEL_INFO, "SBR: %s Length: %ld", ref.string, ref.length);
}