#pragma once

#include "Entity.hpp"

namespace exd
{

struct World
{
	//TODO(zshoals): This struct needs to define a COMPONENT_DATA macro and import all components
	//And also bitsets and tags

	exd::Entity ent_create(void);
	void ent_kill(exd::Entity ent);

	void ent_valid(exd::Entity);

	void ent_has(void); //Note: Might be unnecessary, check component array instead
};

}