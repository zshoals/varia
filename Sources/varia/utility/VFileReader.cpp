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
