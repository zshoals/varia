#include "View.hpp"

#include "World.hpp"

#include "varia/validation.hpp"



//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||               Static Utility Func                                   ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||


static inline exd_component_t * exd_view_unitlocal_component_select(exd_view_t * view, exd::ComponentTypeID type)
{
	return &view->world_reference->components[ComponentTypeID_to_raw(type)];
}



//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||              View Core API                                          ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

void exd_view_initialize(exd_view_t * self, exd_world_t * w)
{
	vds_array_initialize(&self->comp_include);
	vds_array_initialize(&self->comp_exclude);
	vds_array_initialize(&self->comp_type_indices);
	self->shortest_dataset = nullptr;
	self->world_reference = w;
	self->finalized = false;
}

void exd_view_include(exd_view_t * self, exd::ComponentTypeID type)
{
	if (self->finalized) ENSURE_UNREACHABLE("This view has already been finalized!");

	exd_component_t * comp = exd_view_unitlocal_component_select(self, type);
	vds_array_push(&self->comp_include, comp);
	vds_array_push(&self->comp_type_indices, comp->UUID);
}

void exd_view_exclude(exd_view_t * self, exd::ComponentTypeID type)
{
	if (self->finalized) ENSURE_UNREACHABLE("This view has already been finalized!");
	exd_component_t * comp = exd_view_unitlocal_component_select(self, type);
	vds_array_push(&self->comp_exclude, comp);
}

void exd_view_compile(exd_view_t * self)
{
	self->finalized = true;

	ENSURE_UINT_GT_ZERO(vds_array_length(&self->comp_include), "This view did not include any components.");
	//Note(zshoals Dec-27-2022):> Not having any exclusions is alright however

	self->shortest_dataset = *(vds_array_get_mut(&self->comp_include, 0));

	size_t to_remove = 0;
	for_range_var(i, vds_array_length(&self->comp_include))
	{
		exd_component_t * comp = *(vds_array_get_mut(&self->comp_include, i));
		if (comp->active_entities <= self->shortest_dataset->active_entities)
		{
			self->shortest_dataset = comp;
			to_remove = i;
		}
	}

	//Note(zshoals 01-29-2023):> Verify that we haven't got one component that is both included and excluded at the same time
	//that would almost certainly be an error
	for_range_var(i, vds_array_length(&self->comp_type_indices))
	{
		exd::ComponentTypeID id = *(vds_array_get(&self->comp_type_indices, i));

		for (exd_component_t * const & comp : self->comp_exclude)
		{
			if (comp->UUID == id)
			{
				DEBUG_ENSURE_KILL_NOW("This view included a component that was also excluded. That was probably unintended.");
			}
		}
	}

	//The shortest dataset already has all the entities that we're interested in
	//so we remove this component array from our validity checks for speed
	vds_array_swap_and_pop(&self->comp_include, to_remove);
}





exd_entity_t exd_view_create_ent(exd_view_t * self)
{
	return exd_world_ent_create(self->world_reference);
}

bool exd_view_kill_ent(exd_view_t * self, exd_entity_t ent)
{
	return exd_world_ent_kill(self->world_reference, ent);
}


void const * exd_view_get_comp(exd_view_t * self, exd_entity_t ent, exd::ComponentTypeID type)
{
	//Note(zshoals Dec-28-2022):> We can go faster by adding a field idx that
	//directly selects the component in the view. We then grab the index_of the
	//component via type indices to determine which slot the component is in
	//We also DON'T swap_and_pop the shortest component dataset, and instead
	//check for nullptrs in the "internal_match" function. This is 30% faster
	//compared to this version of comp_get
	//However, a lot more complicated and finnicky

	vds_search_result_t<exd::ComponentTypeID> res = vds_array_find_get(&self->comp_type_indices, &type);
	DEBUG_ENSURE_TRUE(res.was_found, "Tried to access a non-existent component in a view.");
	
	if (res.was_found)
	{
		exd_component_t * comp = *(vds_array_get_unsafe(&self->comp_include, *res.value));
		return exd_component_get_untyped_unchecked(comp, ent);
	}

	return nullptr;
}

void * exd_view_get_comp_mutable(exd_view_t * self, exd_entity_t ent, exd::ComponentTypeID type)
{
	vds_search_result_t<exd::ComponentTypeID> res = vds_array_find_get(&self->comp_type_indices, &type);
	DEBUG_ENSURE_TRUE(res.was_found, "Tried to access a non-existent component in a view.");
	
	if (res.was_found)
	{
		exd_component_t * comp = *(vds_array_get_unsafe(&self->comp_include, *res.value));
		return exd_component_get_untyped_mutable_unchecked(comp, ent);
	}

	return nullptr;
}

void * exd_view_set_comp(exd_view_t * self, exd_entity_t ent, exd::ComponentTypeID type)
{
	return exd_world_comp_set(self->world_reference, ent, type);
}

bool exd_view_remove_comp(exd_view_t * self, exd_entity_t ent, exd::ComponentTypeID type)
{
	return exd_world_comp_remove(self->world_reference, ent, type);
}

