#pragma once

namespace exd
{

enum ComponentIdentifiers_e
{
	#define EXD_TAG(TYPE, FIELD_NAME) FIELD_NAME,
	#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) FIELD_NAME,
	#include "ComponentData.def"

	MAX_COUNT
};

}