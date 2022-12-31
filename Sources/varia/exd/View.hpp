#pragma once

#include "Entity.hpp"
#include "Component.hpp"
#include "ComponentTypes.hpp"
#include "World.hpp"

#include "varia/vcommon.hpp"
#include "varia/ds/StaticArray.hpp"


namespace exd
{

struct World;

struct View
{
	vds::StaticArray<Component *, 8> comp_include;
	vds::StaticArray<ComponentTypeID, 8> comp_type_indices;
	vds::StaticArray<Component *, 8> comp_exclude;
	Component * shortest_dataset;
	World * world_reference;

	bool finalized;




	void initialize(World * w);

	void include(ComponentTypeID type);
	void exclude(ComponentTypeID type);
	void compile(void);

	void const * comp_get(Entity ent, ComponentTypeID type);
	void * comp_get_mutable(Entity ent, ComponentTypeID type);

	// void iterate_forwards(void (*cb)(View * v, Entity ent));
	//Located in the header for (hopefully) inlining purposes
	template<typename T, typename FUNC>
	void iterate_forwards_single(FUNC cb)
	{
		DEBUG_ENSURE_TRUE(this->finalized, "View was not finalized before usage.");
		//Note(zshoals Dec-28-2822):> We add one here as one include is removed for optimization purposes
		//this is a safe check as compiling the view checks for a genuinely empty include list
		//This does not apply to exclude, which we do not modify at all
		DEBUG_ENSURE_UINT_EQUALS(this->comp_include.length() + 1, 1, "Tried to iterate a single element, however, multiple components were included.");
		DEBUG_ENSURE_UINT_EQUALS(this->comp_exclude.length(), 0, "Tried to iterate a single element, however, exclusions were added (not allowed).");

		Component * comp = this->shortest_dataset;
		for_range_var(i, comp->length())
		{
			cb(static_cast<T *>(comp->get_untyped_mutable_direct(i)));
		}
	}

	template<typename FUNC>
	void iterate_forwards(FUNC cb)
	{
		DEBUG_ENSURE_TRUE(this->finalized, "View was not finalized before usage.");

		size_t const len = this->shortest_dataset->dense_ents.length();
		for_range_var(i, len)
		{
			Entity e = *this->shortest_dataset->dense_ents.get_unsafe(i);
			if (this->internal_target_matches_query(e))
			{
				cb(this, e);
			}
		}
	}

	template<typename T, typename FUNC>
	void iterate_backwards_single(FUNC cb)
	{
		DEBUG_ENSURE_TRUE(this->finalized, "View was not finalized before usage.");
		DEBUG_ENSURE_UINT_EQUALS(this->comp_include.length() + 1, 1, "Tried to iterate a single element, however, multiple components were included.");
		DEBUG_ENSURE_UINT_EQUALS(this->comp_exclude.length(), 0, "Tried to iterate a single element, however, exclusions were added (not allowed).");

		Component * comp = this->shortest_dataset;
		//Note(zshoals Dec-28-2022):> preload the length as we may remove elements while traversing backwards
		//which would reduce the start value and cause untold havoc, probably
		size_t const len = comp->length();
		for_reverse_range_var(i, len)
		{
			cb(static_cast<T *>(comp->get_untyped_mutable_direct(i)));
		}
	}

	private:
	bool internal_target_matches_query(Entity ent);

};

}