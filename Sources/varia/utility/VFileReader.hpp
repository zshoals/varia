#pragma once

#include "varia/VShared.hpp"
#include "varia/ds/VDS-Arena.hpp"
#include "kinc/io/filereader.h"

struct File_Data
{
    void * raw_memory;
    Integer_64 requested_size;
    Boolean loaded;
};

File_Data v_filereader_try_load(VDS_Arena * arena, char const * path);