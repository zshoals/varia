#pragma once

#include "EntityManifest.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "ComponentIdentifiers.hpp"

#define EXD_IMPORT_COMPONENT_INCLUDES
	#include "ComponentData.def"
#undef EXD_IMPORT_COMPONENT_INCLUDES

namespace exd
{

struct World
{
	//Entity manifest
	EntityManifest manifest;
	vds::StaticArray<vds::Bitset32<Constants::exd_max_components>, Constants::exd_max_entities> ent_comps_bitset;
	size_t active_entities;
	//Component arrays
	//Tag arrays
	#pragma warning(push)
	#pragma warning(disable: 4005)

	#define EXD_TAG(TYPE, FIELD_NAME) Tag<TYPE> FIELD_NAME;
	#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) exd::Component<TYPE> FIELD_NAME = {&this->ent_comps_bitset};
	#include "ComponentData.def"

	#pragma warning(pop)

	World(void)
	{
		active_entities = 0;

		size_t counter = 0;
		#pragma warning(push)
		#pragma warning(disable: 4005)

		#define EXD_TAG(TYPE, FIELD_NAME) Tag<TYPE> FIELD_NAME.internal_setUID(static_cast<ComponentIdentifiers_e>(counter)); ++counter;
		#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) FIELD_NAME.internal_setUID(static_cast<ComponentIdentifiers_e>(counter)); ++counter;
		#include "ComponentData.def"

		#pragma warning(pop)
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
		vds::Bitset32<Constants::exd_max_components> * comp_bitset = ent_comps_bitset.get_mut(ent.id_extract());
		vds::StaticArray<u64, Constants::exd_max_components> indices;

		for_range_var(i, Constants::exd_max_components)
		{
			if (comp_bitset->is_set(i))
			{
				indices.push(i);
			}
		}

		for(u64 const & idx : indices)
		{

			switch (idx)
			{
				#pragma warning(push)
				#pragma warning(disable: 4005)

				#define EXD_TAG(TYPE, FIELD_NAME)
				#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME)\
					case ComponentIdentifiers_e::FIELD_NAME:\
					FIELD_NAME.remove(ent);\
					break;
				#include "ComponentData.def"

				#pragma warning(pop)

				default: ENSURE_UNREACHABLE("Should not have hit a default case.");
			}
		}

	}

	//funcs
	//Generate entity
	//Kill entity
	//validity check? 
	//remove entity from all components (generate big switch case)
};

}