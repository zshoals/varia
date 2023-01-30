#pragma once

#include "Entity.hpp"
#include "Component.hpp"
#include "ComponentTypes.hpp"
#include "World.hpp"

#include "varia/vcommon.hpp"
#include "varia/ds/StaticArray.hpp"


struct exd_world_t;

struct exd_view_t
{
	vds_array_t<exd_component_t *, 8> comp_include;
	vds_array_t<exd::ComponentTypeID, 8> comp_type_indices;
	vds_array_t<exd_component_t *, 8> comp_exclude;
	exd_component_t * shortest_dataset;
	exd_world_t * world_reference;

	bool finalized;
};



void exd_view_initialize(exd_view_t * self, exd_world_t * w);

void exd_view_include(exd_view_t * self, exd::ComponentTypeID type);
void exd_view_exclude(exd_view_t * self, exd::ComponentTypeID type);
void exd_view_compile(exd_view_t * self);

exd_entity_t exd_view_create_ent(exd_view_t * self);
bool exd_view_kill_ent(exd_view_t * self, exd_entity_t ent);

void const * exd_view_get_comp(exd_view_t * self, exd_entity_t ent, exd::ComponentTypeID type);
void * exd_view_get_comp_mutable(exd_view_t * self, exd_entity_t ent, exd::ComponentTypeID type);
void * exd_view_set_comp(exd_view_t * self, exd_entity_t ent, exd::ComponentTypeID type);
bool exd_view_remove_comp(exd_view_t * self, exd_entity_t ent, exd::ComponentTypeID type);


//Note(zshoals 01-30-2023):> This function is likely a hotspot so it's been moved to the header
//for performance reasons (we're hoping for inlining here)
inline bool exd_view_internal_entity_matches_query_requirements(exd_view_t * self, exd_entity_t ent)
{

	//If the entity DOES NOT HAVE an inclusion target, no match
	for (exd_component_t * const & included_comp : self->comp_include)
	{
		if (!exd_component_has(included_comp, ent))
		{
			return false;
		}
	}

	//If the entity HAS an exclusion target, no match
	for (exd_component_t * const & excluded_comp : self->comp_exclude)
	{
		if (!exd_component_has(excluded_comp, ent))
		{
			return false;
		}
	}

	return true;
}


//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                   Forwards Iteration                                ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

//TODO(zshoals 01-03-2023):> For SIMD purposes we may want to make a 4step iteration
//mechanism.

//Located in the header for (hopefully) inlining purposes
template<typename T, typename FUNC>
void exd_view_iterate_forwards_single(exd_view_t * self, FUNC cb)
{
	DEBUG_ENSURE_TRUE(self->finalized, "View was not finalized before usage.");
	//Note(zshoals Dec-28-2822):> We add one here as one include is removed for optimization purposes
	//this is a safe check as compiling the view checks for a genuinely empty include list
	//This does not apply to exclude, which we do not modify at all
	DEBUG_ENSURE_UINT_EQUALS(vds_array_length(&self->comp_include) + 1, 1, "Tried to iterate a single element, however, multiple components were included.");
	DEBUG_ENSURE_UINT_EQUALS(vds_array_length(&self->comp_exclude), 0, "Tried to iterate a single element, however, exclusions were added (not allowed).");

	exd_component_t * comp = self->shortest_dataset;

	DEBUG_ENSURE_UINT_EQUALS(self->shortest_dataset->per_element_size, sizeof(T), "Created a view that is trying to use a type that is not equivalent to the target component array.");

	const size_t len = exd_component_length(comp);
	for_range_var(i, len)
	{
		// cb(static_cast<T *>(comp->get_untyped_mutable_direct(i)));
		cb(static_cast<T *>(exd_component_get_untyped_mutable_direct(comp, i)));
	}
}

template<typename T, typename FUNC>
void exd_view_iterate_forwards_single_with_entity(exd_view_t * self, FUNC cb)
{
	DEBUG_ENSURE_TRUE(self->finalized, "View was not finalized before usage.");
	//Note(zshoals Dec-28-2822):> We add one here as one include is removed for optimization purposes
	//this is a safe check as compiling the view checks for a genuinely empty include list
	//This does not apply to exclude, which we do not modify at all
	DEBUG_ENSURE_UINT_EQUALS(vds_array_length(&self->comp_include) + 1, 1, "Tried to iterate a single element, however, multiple components were included.");
	DEBUG_ENSURE_UINT_EQUALS(vds_array_length(&self->comp_exclude), 0, "Tried to iterate a single element, however, exclusions were added (not allowed).");

	exd_component_t * comp = self->shortest_dataset;

	DEBUG_ENSURE_UINT_EQUALS(self->shortest_dataset->per_element_size, sizeof(T), "Created a view that is trying to use a type that is not equivalent to the target component array.");

	const size_t len = exd_component_length(comp);
	for_range_var(i, len)
	{
		exd_entity_t e = vds_array_get_unsafe(&comp->dense_ents, i);
		// cb(self, static_cast<T *>(comp->get_untyped_mutable_direct(i)), e);
		cb(self, static_cast<T *>(exd_component_get_untyped_mutable_direct(comp, i)), e);
	}
}

template<typename FUNC>
void exd_view_iterate_forwards(exd_view_t * self, FUNC cb)
{
	DEBUG_ENSURE_TRUE(self->finalized, "View was not finalized before usage.");

	//TODO(zshoals 01-29-2023):> Verify that dense_ents length == exd_component_active_entities length
	// size_t const len = self->shortest_dataset->dense_ents.length();
	size_t const len = vds_array_length(&self->shortest_dataset->dense_ents);
	for_range_var(i, len)
	{
		// exd_entity_t e = *self->shortest_dataset->dense_ents.get_unsafe(i);
		exd_entity_t e = *(vds_array_get_unsafe(self->shortest_dataset, i));
		if (exd_view_internal_entity_matches_query_requirements(self, e))
		{
			cb(self, e);
		}
	}
}



//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                       Backwards Iteration                           ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
template<typename T, typename FUNC>
void exd_view_iterate_backwards_single(exd_view_t * self, FUNC cb)
{
	DEBUG_ENSURE_TRUE(self->finalized, "View was not finalized before usage.");
	DEBUG_ENSURE_UINT_EQUALS(vds_array_length(&self->comp_include) + 1, 1, "Tried to iterate a single element, however, multiple components were included.");
	DEBUG_ENSURE_UINT_EQUALS(vds_array_length(&self->comp_exclude), 0, "Tried to iterate a single element, however, exclusions were added (not allowed).");

	exd_component_t * comp = self->shortest_dataset;

	DEBUG_ENSURE_UINT_EQUALS(self->shortest_dataset->per_element_size, sizeof(T), "Created a view that is trying to use a type that is not equivalent to the target component array.");
	//Note(zshoals Dec-28-2022):> preload the length as we may remove elements while traversing backwards
	//which would reduce the start value and cause untold havoc, probably
	size_t const len = exd_component_length(comp);
	for_reverse_range_var(i, len)
	{
		cb(static_cast<T *>(exd_component_get_untyped_mutable_direct(comp, i)));
	}
}

template<typename T, typename FUNC>
void exd_view_iterate_backwards_single_with_entity(exd_view_t * self, FUNC cb)
{
	DEBUG_ENSURE_TRUE(self->finalized, "View was not finalized before usage.");
	DEBUG_ENSURE_UINT_EQUALS(vds_array_length(&self->comp_include) + 1, 1, "Tried to iterate a single element, however, multiple components were included.");
	DEBUG_ENSURE_UINT_EQUALS(vds_array_length(&self->comp_exclude), 0, "Tried to iterate a single element, however, exclusions were added (not allowed).");

	exd_component_t * comp = self->shortest_dataset;

	DEBUG_ENSURE_UINT_EQUALS(self->shortest_dataset->per_element_size, sizeof(T), "Created a view that is trying to use a type that is not equivalent to the target component array.");
	//Note(zshoals Dec-28-2022):> preload the length as we may remove elements while traversing backwards
	//which would reduce the start value and cause untold havoc, probably
	size_t const len = component_length(comp);
	for_reverse_range_var(i, len)
	{
		exd_entity_t ent = *(vds_array_get_unsafe(&comp->dense_ents, i));
		cb(self, static_cast<T *>(exd_component_get_untyped_mutable_direct(comp, i)), ent);
	}
}

template<typename FUNC>
void exd_view_iterate_backwards(exd_view_t * self, FUNC cb)
{
	DEBUG_ENSURE_TRUE(self->finalized, "View was not finalized before usage.");

	size_t const len = vds_array_length(&self->shortest_dataset->dense_ents);
	for_reverse_range_var(i, len)
	{
		exd_entity_t e = *(vds_get_unsafe(&self->shortest_dataset->dense_ents, i));
		if (exd_view_internal_entity_matches_query_requirements(self, e))
		{
			cb(self, e);
		}
	}

}
