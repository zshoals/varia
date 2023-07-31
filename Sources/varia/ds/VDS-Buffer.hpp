#pragma once

#include "VDS-Types.hpp"
//Memcpy
#include <string.h>
#include "kinc/io/filereader.h"

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

static inline Boolean vds_buffer_read_asset_from_file(VDS_Buffer * buffer, char const * path)
{
    kinc_file_reader_t reader;

    if ( !kinc_file_reader_open(&(reader), path, KINC_FILE_TYPE_ASSET) )
    {
        return false;
    }

    size_t incoming_size = kinc_file_reader_size(&(reader));
    if ((Integer_64)(incoming_size) > buffer->capacity)
    {
        kinc_file_reader_close(&reader);
        return false;
    }

    Integer_64 written_bytes = kinc_file_reader_read(&reader, &(buffer->data[0]), incoming_size);
    *(buffer->data_written) = written_bytes;

    kinc_file_reader_close(&reader);

    return true;
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