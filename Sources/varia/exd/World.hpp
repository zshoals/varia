#pragma once

#include "EntityManifest.hpp"
#include "Entity.hpp"
#include "Component.hpp"

#define EXD_IMPORT_COMPONENT_INCLUDES
	#include "ComponentData.def"
#undef EXD_IMPORT_COMPONENT_INCLUDES

namespace exd
{

struct World
{
	//Entity manifest
	EntityManifest manifest;
	size_t active_entities;
	//Component arrays
	//Tag arrays
	#define EXD_TAG(TYPE, FIELD_NAME) Tag<TYPE> FIELD_NAME;
	#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) exd::Component<TYPE> FIELD_NAME;
	#include "ComponentData.def"

	World(void)
	{
		active_entities = 0;
	}

	Entity ent_create(void)
	{
		++active_entities;
		return manifest.get_free();
	}

	bool ent_kill(Entity ent)
	{
		bool removed = manifest.release(ent);
		if (removed)
		{
			internal_ent_remove_from_components(ent);
			--active_entities;
			return true;
		}

		return false;
	}

	bool ent_valid(Entity ent)
	{
		return manifest.valid(ent);
	}

	void internal_ent_remove_from_components(Entity ent)
	{
		//TODO(zshoals Dec-20-2022):>
		//Moronically try and remove this ent from every array for now
		//Replace with switch case and generated list of targets to remove from
		#define EXD_TAG(TYPE, FIELD_NAME)
		#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) FIELD_NAME.remove(ent);
		#include "ComponentData.def"
	}

	//funcs
	//Generate entity
	//Kill entity
	//validity check? 
	//remove entity from all components (generate big switch case)
};

}