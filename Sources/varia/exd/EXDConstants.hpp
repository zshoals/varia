#pragma once

#include "varia/vcommon.hpp"
#include "ComponentTypes.hpp"
#include "EXDUtil.hpp"

//Note(zshoals Dec-22-2022):> Numbers greater than 65536 actually cause entity creation
//to be faster by a factor of 2 almost. Wtf?
constexpr size_t EXD_GLOBAL_CONSTANTS_MAX_ENTITIES = 81920;
constexpr size_t EXD_GLOBAL_CONSTANTS_MAX_COMPONENTS = exd::ComponentTypeID::MAX_COUNT;
constexpr u8 EXD_GLOBAL_CONSTANTS_ID_SHIFT = exd_utility_calculate_id_shift(EXD_GLOBAL_CONSTANTS_MAX_ENTITIES);
