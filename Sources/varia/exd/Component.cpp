#include "Component.hpp"
#include "ComponentTypes.hpp"
#include "Entity.hpp"

#include "varia/vcommon.hpp"
#include "varia/logging.hpp"
#include "varia/validation.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/util/Memory.hpp"
#include <inttypes.h>

// exd::Component::Component(void) {}

void exd_component_initialize(exd_component_t * comp, void * mem, size_t element_size, size_t element_count, exd::ComponentTypeID UUID)
{
	comp->data = mem;
	comp->per_element_size = element_size;
	comp->element_count = element_count;
	comp->UUID = UUID;
	comp->active_entities = 0;

	vds_array_set_all(&comp->sparse_ents, exd::INVALID_ENTITY.id);

	comp->memory_usage_in_bytes = (element_size * element_count) + sizeof(*comp);
}

size_t exd_component_length(exd_component_t * comp)
{
	return comp->active_entities;
}

void * exd_component_calc_element_address(exd_component_t * comp, exd_entity_t ent)
{
	u64 idx = exd_entity_extract_id(ent);
	u64 target_offset = idx * comp->per_element_size;

	DEBUG_ENSURE_UINT_LT(target_offset, comp->element_count * comp->per_element_size, "Untyped component access overran buffer.");

	uintptr_t target_addr = reinterpret_cast<uintptr_t>(comp->data) + target_offset;

	return reinterpret_cast<void *>(target_addr);
}

void * exd_component_calc_element_address_raw(exd_component_t * comp, size_t idx)
{
	u64 target_offset = idx * comp->per_element_size;

	DEBUG_ENSURE_UINT_LT(target_offset, comp->element_count * comp->per_element_size, "Untyped component access overran buffer.");

	uintptr_t target_addr = reinterpret_cast<uintptr_t>(comp->data) + target_offset;

	return reinterpret_cast<void *>(target_addr);
}

size_t exd_component_back(exd_component_t * comp)
{
	return comp->active_entities - 1;
}

size_t exd_component_front(exd_component_t * comp)
{
	return 0;
}

void exd_component_push_comp(exd_component_t * comp, void const * data)
{
	//TODO(zshoals 01-27-2023):> Did component need to be passed in here or just the data?
	void * elem = exd_component_calc_element_address_raw(comp, comp->active_entities);
	memcpy(elem, data, comp->per_element_size);

	++comp->active_entities;
}

void exd_component_push_comp_without_data(exd_component_t * comp)
{
	++comp->active_entities;
}

void exd_component_swap_and_pop_comp(exd_component_t * comp, size_t idx)
{
	DEBUG_ENSURE_INT_GT_ZERO(comp->active_entities, "Component array had no entities!");
	void * removal_target = exd_component_calc_element_address_raw(comp, idx);
	void * rear = exd_component_calc_element_address_raw(comp, exd_component_back(comp));

	//Note(zshoals Dec-25-2022):> We don't care about the target element once it's been removed
	//from the data array, which means that we do not need to move the removed item
	//to the rear of the array
	//Instead, only shift the rear into the removed slot and essentially duplicate the rear data.
	memcpy(removal_target, rear, comp->per_element_size);

	--comp->active_entities;
}

bool exd_component_has(exd_component_t * comp, exd_entity_t ent)
{
	u64 id = exd_entity_extract_id(ent);
	size_t target_idx = *(vds_array_get_unsafe(&comp->sparse_ents, id));

	if (target_idx != exd::INVALID_ENTITY.id)
	{
		exd_entity_t other = *(vds_array_get_unsafe(&comp->dense_ents, target_idx));
		return exd_entity_matches(ent, other);
	}

	return false;
}

void const * exd_component_get_untyped(exd_component_t * comp, exd_entity_t ent)
{
	if (exd_component_has(comp, ent))
	{
		size_t target_idx = *(vds_array_get_unsafe(&comp->sparse_ents, exd_entity_extract_id(ent)));
		return exd_component_calc_element_address_raw(comp, target_idx);
	}
	// ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

	return nullptr;
}

void * exd_component_get_untyped_mutable(exd_component_t * comp, exd_entity_t ent)
{
	if (exd_component_has(comp, ent))
	{
		size_t target_idx = *(vds_array_get_unsafe(&comp->sparse_ents, exd_entity_extract_id(ent)));
		return exd_component_calc_element_address_raw(comp, target_idx);
	}
	// ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

	return nullptr;
}

void const * exd_component_get_untyped_unchecked(exd_component_t * comp, exd_entity_t ent)
{
	return exd_component_calc_element_address(comp, ent);
}

void * exd_component_get_untyped_mutable_unchecked(exd_component_t * comp, exd_entity_t ent)
{
	return exd_component_calc_element_address(comp, ent);
}

void const * exd_component_get_untyped_direct(exd_component_t * comp, size_t idx)
{
	return exd_component_calc_element_address_raw(comp, idx);
}

void * exd_component_get_untyped_mutable_direct(exd_component_t * comp, size_t idx)
{
	return exd_component_calc_element_address_raw(comp, idx);
}

bool exd_component_add_entity(exd_component_t * comp, exd_entity_t ent)
{

	//TODO(zshoals Dec-27-2022):> There's a problem; if we try and add a STORED entity to a component
	//and that entity is out of date, addition should be an error. We actually need to check while
	//adding if the entity is the newest version according to the world manifest, I think
	//Which means we need to mirror the generation on the sparse ent slot I believe
	//NOTE(zshoals Dec-27-2022):> Fixed in world comp_set???? maybe??? 
	u64 id = exd_entity_extract_id(ent);

	if ( *(vds_array_get_unsafe(&comp->sparse_ents, id)) != exd::INVALID_ENTITY.id)
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to add an entity into this component, but it already exists. ID (no generation): %zu", id);
		return false;
	}

	vds_array_push(&comp->dense_ents, ent);

	//TODO(zshoals 01-28-2023):> These two lines are not resolved yet as exd_entity_t is not defined properly yet
	// sparse_ents.set_unsafe(id, dense_ents.back());
	vds_array_set_unsafe(&comp->sparse_ents, id, vds_array_back(&comp->dense_ents));
	exd_component_push_comp_without_data(comp);

	return true;
}

bool exd_component_remove_entity(exd_component_t * comp, exd_entity_t ent)
{
	if (comp->active_entities < 1)
	{
		// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		return false;
	}

	u64 id = exd_entity_extract_id(ent);

	size_t target_idx = *(vds_array_get_unsafe(&comp->sparse_ents, id));
	if (target_idx == exd::INVALID_ENTITY.id) 
	{
		// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		return false;
	};

	exd_entity_t dense_ent = *(vds_array_get_unsafe(&comp->dense_ents, target_idx));

	bool valid = exd_entity_matches(ent, dense_ent);
	if (valid)
	{
		vds_array_swap_and_pop(&comp->dense_ents, target_idx);
		exd_component_swap_and_pop_comp(comp, target_idx);

		exd_entity_t reciprocal = *(vds_array_get_unsafe(&comp->dense_ents, target_idx));

		//Note(zshoals Dec-19-2022):> Order is important here, if the element is the final one
		//in the array, reciprocal and ent id resolve to the same thing. So set INVALID_ENTITY.id
		//second
		vds_array_set_unsafe(&comp->sparse_ents, exd_entity_extract_id(reciprocal), target_idx);
		vds_array_set_unsafe(&comp->sparse_ents, exd_entity_extract_id(ent), exd::INVALID_ENTITY.id);

		return true;
	}
	else
	{
		// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		return false;
	}
}

void exd_component_clear(exd_component_t * comp)
{
	ENSURE_UNIMPLEMENTED();	
	memset(comp->data, 0, (comp->element_count * comp->per_element_size));
}
