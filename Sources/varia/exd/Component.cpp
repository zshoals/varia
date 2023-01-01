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

void exd::Component::initialize(void * mem, size_t element_size, size_t element_count, ComponentTypeID UUID)
{
	this->data = mem;
	this->per_element_size = element_size;
	this->element_count = element_count;
	this->UUID = UUID;
	this->active_entities = 0;

	sparse_ents.set_all(INVALID_ENTITY.id);

	this->memory_usage_in_bytes = (element_size * element_count) + sizeof(*this);
}

size_t exd::Component::length(void)
{
	return this->active_entities;
}

void * exd::Component::calc_element_address(Entity ent)
{
	u64 idx = ent.id_extract();
	u64 target_offset = idx * per_element_size;

	DEBUG_ENSURE_UINT_LT(target_offset, element_count * per_element_size, "Untyped component access overran buffer.");

	uintptr_t target_addr = reinterpret_cast<uintptr_t>(this->data) + target_offset;

	return reinterpret_cast<void *>(target_addr);
}

void * exd::Component::calc_element_address_raw(size_t idx)
{
	u64 target_offset = idx * per_element_size;

	DEBUG_ENSURE_UINT_LT(target_offset, element_count * per_element_size, "Untyped component access overran buffer.");

	uintptr_t target_addr = reinterpret_cast<uintptr_t>(this->data) + target_offset;

	return reinterpret_cast<void *>(target_addr);
}

size_t exd::Component::back(void)
{
	return this->active_entities - 1;
}

size_t exd::Component::front(void)
{
	return 0;
}

void exd::Component::push_comp(void const * data)
{
	void * elem = calc_element_address_raw(this->active_entities);
	memcpy(elem, data, this->per_element_size);

	++this->active_entities;
}

void exd::Component::push_comp_without_data(void)
{
	++this->active_entities;
}

void exd::Component::swap_and_pop_comp(size_t idx)
{
	DEBUG_ENSURE_INT_GT_ZERO(this->active_entities, "Component array had no entities!");
	void * removal_target = calc_element_address_raw(idx);
	void * rear = calc_element_address_raw(back());

	//Note(zshoals Dec-25-2022):> We don't care about the target element once it's been removed
	//from the data array, which means that we do not need to move the removed item
	//to the rear of the array
	//Instead, only shift the rear into the removed slot and essentially duplicate the rear data.
	memcpy(removal_target, rear, this->per_element_size);

	--this->active_entities;
}

bool exd::Component::has(Entity ent)
{
	u64 id = ent.id_extract();
	size_t target_idx = *sparse_ents.get_unsafe(id);

	if (target_idx != INVALID_ENTITY.id)
	{
		Entity other = *dense_ents.get_unsafe(target_idx);
		return ent.matches(other);
	}

	return false;
}

void const * exd::Component::get_untyped(Entity ent)
{
	if (has(ent))
	{
		size_t target_idx = *sparse_ents.get_unsafe(ent.id_extract());
		return calc_element_address_raw(target_idx);
	}
	// ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

	return nullptr;
}

void * exd::Component::get_untyped_mutable(Entity ent)
{
	if (has(ent))
	{
		size_t target_idx = *sparse_ents.get_unsafe(ent.id_extract());
		return calc_element_address_raw(target_idx);
	}
	// ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

	return nullptr;
}

void const * exd::Component::get_untyped_unchecked(Entity ent)
{
	return calc_element_address(ent);
}

void * exd::Component::get_untyped_mutable_unchecked(Entity ent)
{
	return calc_element_address(ent);
}

void const * exd::Component::get_untyped_direct(size_t idx)
{
	return calc_element_address_raw(idx);
}

void * exd::Component::get_untyped_mutable_direct(size_t idx)
{
	return calc_element_address_raw(idx);
}

void exd::Component::entity_add(Entity ent)
{

	//TODO(zshoals Dec-27-2022):> There's a problem; if we try and add a STORED entity to a component
	//and that entity is out of date, addition should be an error. We actually need to check while
	//adding if the entity is the newest version according to the world manifest, I think
	//Which means we need to mirror the generation on the sparse ent slot I believe
	//NOTE(zshoals Dec-27-2022):> Fixed in world comp_set???? maybe??? 
	u64 id = ent.id_extract();

	if (*sparse_ents.get_unsafe(id) != INVALID_ENTITY.id)
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to add an entity into this component, but it already exists. ID (no generation): %zu", ent.id_extract());
		return;
	}

	dense_ents.push(ent);
	sparse_ents.set_unsafe(id, dense_ents.back());
	this->push_comp_without_data();

	//TODO(zshoals Dec-24-2022):> No entset added yet. Do we still need one?
	// internal_add_this_comp_to_entset(id);

}

bool exd::Component::entity_remove(Entity ent)
{
	if (active_entities < 1)
	{
		// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		return false;
	}

	u64 id = ent.id_extract();

	size_t target_idx = *sparse_ents.get_unsafe(id);
	if (target_idx == INVALID_ENTITY.id) 
	{
		// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		return false;
	};

	Entity dense_ent = *dense_ents.get_unsafe(target_idx);

	bool valid = ent.matches(dense_ent);
	if (valid)
	{
		dense_ents.swap_and_pop(target_idx);
		this->swap_and_pop_comp(target_idx);

		Entity reciprocal = *dense_ents.get_unsafe(target_idx);

		//Note(zshoals Dec-19-2022):> Order is important here, if the element is the final one
		//in the array, reciprocal and ent id resolve to the same thing. So set INVALID_ENTITY.id
		//second
		sparse_ents.set_unsafe(reciprocal.id_extract(), target_idx);
		sparse_ents.set_unsafe(ent.id_extract(), INVALID_ENTITY.id);

		//TODO(zshoals):>URGENT! FIXME!!!
		// internal_remove_this_comp_from_entset(id);


		return true;
	}
	else
	{
		// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		return false;
	}
}

void exd::Component::clear(void)
{
	ENSURE_UNIMPLEMENTED();	
	memset(this->data, 0, element_count * per_element_size);
}
