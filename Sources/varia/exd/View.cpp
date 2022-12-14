#include "View.hpp"

#include "World.hpp"

#include "varia/validation.hpp"

void exd::View::initialize(World * w)
{
	this->comp_include.initialize();
	this->comp_exclude.initialize();
	this->comp_type_indices.initialize();
	this->shortest_dataset = nullptr;
	this->world_reference = w;
	this->finalized = false;
}

void exd::View::include(ComponentTypeID type)
{
	if (finalized) ENSURE_UNREACHABLE("This view has already been finalized!");

	Component * comp = &this->world_reference->components[ComponentTypeID_to_raw(type)];
	comp_include.push(comp);
	comp_type_indices.push(comp->UUID);
}

void exd::View::exclude(ComponentTypeID type)
{
	if (finalized) ENSURE_UNREACHABLE("This view has already been finalized!");
	Component * comp = &this->world_reference->components[ComponentTypeID_to_raw(type)];
	comp_exclude.push(comp);
}

void exd::View::compile(void)
{
	finalized = true;

	ENSURE_UINT_GT_ZERO(comp_include.length(), "This view did not include any components.");
	//Note(zshoals Dec-27-2022):> Not having any exclusions is alright however

	shortest_dataset = *comp_include.get_mut(0);

	size_t to_remove = 0;
	for_range_var(i, comp_include.length())
	{
		Component * comp = *comp_include.get_mut(i);
		if (comp->active_entities <= shortest_dataset->active_entities)
		{
			shortest_dataset = comp;
			to_remove = i;
		}
	}

	for_range_var(i, comp_type_indices.length())
	{
		ComponentTypeID id = *comp_type_indices.get(i);

		for (Component * const & comp : comp_exclude)
		{
			if (comp->UUID == id)
			{
				DEBUG_ENSURE_KILL_NOW("This view included a component that was also excluded. That was probably unintended.");
			}
		}
	}

	//The shortest dataset already has all the entities that we're interested in
	//so we remove this component array from our validity checks for speed
	comp_include.swap_and_pop(to_remove);
}





exd::Entity exd::View::ent_create(void)
{
	return this->world_reference->ent_create();
}

bool exd::View::ent_kill(exd::Entity ent)
{
	return this->world_reference->ent_kill(ent);
}


void const * exd::View::comp_get(Entity ent, ComponentTypeID type)
{
	//Note(zshoals Dec-28-2022):> We can go faster by adding a field idx that
	//directly selects the component in the view. We then grab the index_of the
	//component via type indices to determine which slot the component is in
	//We also DON'T swap_and_pop the shortest component dataset, and instead
	//check for nullptrs in the "internal_match" function. This is 30% faster
	//compared to this version of comp_get
	//However, a lot more complicated and finnicky

	vds::SearchResult<ComponentTypeID> res = comp_type_indices.find_get(&type);
	DEBUG_ENSURE_TRUE(res.was_found, "Tried to access a non-existent component in a view.");
	
	if (res.was_found)
	{
		Component * comp = *comp_include.get_unsafe(*res.value);
		return comp->get_untyped_unchecked(ent);
	}

	return nullptr;
}

void * exd::View::comp_get_mutable(Entity ent, ComponentTypeID type)
{
	vds::SearchResult<ComponentTypeID> res = comp_type_indices.find_get(&type);
	DEBUG_ENSURE_TRUE(res.was_found, "Tried to access a non-existent component in a view.");
	
	if (res.was_found)
	{
		Component * comp = *comp_include.get_unsafe(*res.value);
		return comp->get_untyped_mutable_unchecked(ent);
	}

	return nullptr;
}

void * exd::View::comp_set(Entity ent, ComponentTypeID type)
{
	return this->world_reference->comp_set(ent, type);
}

bool exd::View::comp_remove(Entity ent, ComponentTypeID type)
{
	return this->world_reference->comp_remove(ent, type);
}




//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                      Internal usage only                            ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||


bool exd::View::internal_target_matches_query(Entity ent)
{

	//If the entity DOES NOT HAVE an inclusion target, no match
	for (Component * const & comp : comp_include)
	{
		if (!comp->has(ent))
		{
			return false;
		}
	}

	//If the entity HAS an exclusion target, no match
	for (Component * const & comp : comp_exclude)
	{
		if (comp->has(ent))
		{
			return false;
		}
	}

	return true;
}