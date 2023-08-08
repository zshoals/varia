#include "varia/ds/VDS-Arena.hpp"
#include "varia/ds/VDS-StringBuffer.hpp"

//Strings
using Permanent_String_Buffer = VDS_String_Buffer<VARIA_MEGABYTES(16)>;
using Temporary_String_Buffer = VDS_String_Buffer<VARIA_MEGABYTES(8)>;

//Arbitrary memory storage
using Permanent_Storage_Arena = VDS_Arena<VARIA_MEGABYTES(32)>;
using Temporary_Storage_Arena = VDS_Arena<VARIA_MEGABYTES(160)>;
