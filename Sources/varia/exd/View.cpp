#include "View.hpp"

#include "varia/validation.hpp"

void exd::View::initialize(void)
{
	this->comp_include.initialize();
	this->comp_exclude.initialize();
	this->comp_type_indices.initialize();
	this->shortest_dataset = nullptr;
	this->finalized = false;
}

void exd::View::include(Component * comp)
{
	if (finalized) ENSURE_UNREACHABLE("This view has already been finalized!");
	comp_include.push(comp);
	comp_type_indices.push(comp->UUID);
}

void exd::View::exclude(Component * comp)
{
	if (finalized) ENSURE_UNREACHABLE("This view has already been finalized!");
	comp_exclude.push(comp);
}

void exd::View::compile(void)
{
	finalized = true;

	ENSURE_UINT_GT_ZERO(comp_include.length(), "This view did not include any components.");
	//Note(zshoals Dec-27-2022):> Not having any exclusions is alright however

	shortest_dataset = *comp_include.get_mut(0);

	for(Component * const & comp : comp_include)
	{
		if (comp->active_entities <= shortest_dataset->active_entities)
		{
			shortest_dataset = comp;
		}
	}
}

void const * exd::View::comp_get(Entity ent, ComponentTypeID type)
{
	//Note(zshoals Dec-27-2022):> Findget is mostly useful in debug
	//We take a noteworthy performance hit (~20% slowdown?) from using that path
	//Considering the slowdown, this is worth ifdefing out

	#ifdef NDEBUG

	Component * comp = *comp_include.get_unsafe(ComponentTypeID_to_raw(type));
	return comp->get_untyped_unchecked(ent);

	#else

	vds::SearchResult<ComponentTypeID> res = comp_type_indices.find_get(&type);
	DEBUG_ENSURE_TRUE(res.was_found, "Tried to access a non-existent component in a view.");
	
	if (res.was_found)
	{
		Component * comp = *comp_include.get_unsafe(*res.value);
		return comp->get_untyped_unchecked(ent);
	}

	return nullptr;

	#endif
}

void * exd::View::comp_get_mutable(Entity ent, ComponentTypeID type)
{
	#ifdef NDEBUG

	Component * comp = *comp_include.get_unsafe(ComponentTypeID_to_raw(type));
	return comp->get_untyped_mutable_unchecked(ent);

	#else

	vds::SearchResult<ComponentTypeID> res = comp_type_indices.find_get(&type);
	DEBUG_ENSURE_TRUE(res.was_found, "Tried to access a non-existent component in a view.");
	
	if (res.was_found)
	{
		Component * comp = *comp_include.get_unsafe(*res.value);
		return comp->get_untyped_mutable_unchecked(ent);
	}

	return nullptr;

	#endif
}


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