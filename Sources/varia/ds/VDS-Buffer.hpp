#pragma once

#include "VDS-Types.hpp"

template <Integer_64 SIZE>
struct VDS_Buffer_Storage
{
    Integer_64 data_written;
    Byte data[SIZE];
};

struct VDS_Buffer
{
    Integer_64 capacity;
    Integer_64 * data_written;
    Byte * data;
};

template <Integer_64 SIZE>
VDS_Buffer vds_buffer_make_interface(VDS_Buffer_Storage<SIZE> * buffer)
{
    VDS_Buffer interface;
    {
        interface.capacity = SIZE;
        interface.data_written = &(buffer->data_written);
        interface.data = &(buffer->data[0]);
    }

    return interface;
}

static inline Boolean vds_buffer_has_room(VDS_Buffer const * buffer, Integer_64 requested_capacity)
{
    return (buffer->capacity >= requested_capacity);
}

static inline Boolean vds_buffer_write(VDS_Buffer * buffer, char const * data, Integer_64 requested_write_size)
{
    VDS_ASSERT(buffer->capacity >= requested_write_size, "Buffer overrun on write");

    if (buffer->capacity >= requested_write_size)
    {
        Byte * writer = &(buffer->data[0]);
        memcpy(writer, data, requested_write_size);

        *(buffer->data_written) = requested_write_size;

        return true;
    }
    else
    {
        return false;
    }
}

template<typename T>
T * vds_buffer_reinterpret(VDS_Buffer * buffer)
{
    T * object = static_cast<T *>( &(buffer->data[0]) ); 
    VDS_ASSERT(sizeof(*object) == *(buffer->data_written), "Mismatched size on reinterpretation of buffer; error?");

    return object;
}

static inline void vds_buffer_reset(VDS_Buffer * buffer)
{
    *(buffer->data_written) = 0;
}