#include "varia/utility/VFileReader.hpp"

#include "varia/VShared.hpp"
#include "kinc/io/filereader.h"

Boolean v_filereader_can_open(char const * path)
{
    kinc_file_reader_t reader;

    Boolean loaded = kinc_file_reader_open(address_of(reader), path, KINC_FILE_TYPE_ASSET);

    if (loaded)
    {
        kinc_file_reader_close(address_of(reader));
    }

    return loaded;
}

File_Data v_filereader_try_load(VDS_Arena * arena, char const * path)
{
    File_Data file_data = ZERO_INIT();
    kinc_file_reader_t reader;
    file_data.loaded = kinc_file_reader_open(address_of(reader), path, KINC_FILE_TYPE_ASSET);

    if (file_data.loaded)
    {
        file_data.requested_size = kinc_file_reader_size(address_of(reader));
        file_data.raw_memory = vds_arena_allocate(arena, file_data.requested_size);
    }
    else
    {
        file_data.raw_memory = nullptr;
        file_data.requested_size = 0;
        file_data.loaded = false;
    }

    if (file_data.loaded)
    {
        kinc_file_reader_close(address_of(reader));
    }

    return file_data;
}