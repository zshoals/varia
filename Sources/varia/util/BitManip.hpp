#pragma once

#include "varia/vcommon.hpp"
#include <string.h>

#define VARIA_BITCAST(DESTINATION_PTR, SOURCE_PTR)\
	do\
	{\
		memcpy(DESTINATION_PTR, SOURCE_PTR, sizeof(*SOURCE_PTR));\
	} while (0)
	
