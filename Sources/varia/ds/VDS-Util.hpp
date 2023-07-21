//TODO(<zshoals> 07-18-2023): This pragma once might cause problems since assert is meant to be included
//  multiple times; be careful?
#pragma once

#include <assert.h>

#define VDS_ASSERT(COND, MSG)\
    if (!(COND)) { __debugbreak(); assert(0 && (MSG)); }

#define VDS_COUNT_OF(ARRPTR) ( sizeof( (ARRPTR)->data ) / sizeof( (ARRPTR)->data[0]) )