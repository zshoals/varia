#pragma once

#include "varia/vcommon.hpp"

namespace exd
{

enum ComponentTypeID
{
	#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) VARIA_CONCAT(TYPE, _e),
	#include "ComponentData.def"
	MAX_COUNT
};

constexpr static size_t ComponentTypeID_to_raw(ComponentTypeID type)
{
	return static_cast<size_t>(type);
}

constexpr static ComponentTypeID ComponentTypeID_to_enum(size_t type_idx)
{
	return static_cast<ComponentTypeID>(type_idx);
}

}