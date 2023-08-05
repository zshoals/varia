#pragma once

#include <assert.h>

#define VDS_ASSERT(COND, MSG)\
    if (!(COND)) { __debugbreak(); assert(0 && (MSG)); }

#define VDS_COUNT_OF(ARRPTR) ( sizeof( (ARRPTR)->data ) / sizeof( (ARRPTR)->data[0]) )