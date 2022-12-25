#include "Component-U.hpp"
#include "Entity.hpp"

#include "varia/vcommon.hpp"
#include "varia/logging.hpp"
#include "varia/validation.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/util/Memory.hpp"
#include <inttypes.h>

exd::Component::Component(vds::Allocator * mem, size_t element_size, size_t element_count) :
	arena{mem}, per_element_size{element_size}, element_count{element_count}, UUID{0},
	sparse_ents{}, dense_ents{}
{
	this->data = arena->allocate_aligned(element_size * element_count, Varia::Memory::default_alignment);
	sparse_ents.set_all(INVALID_ENTITY.id);
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
	ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

	return nullptr;
}

void * exd::Component::get_untyped_mutable(Entity ent)
{
	if (has(ent))
	{
		size_t target_idx = *sparse_ents.get_unsafe(ent.id_extract());
		return calc_element_address_raw(target_idx);
	}
	ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

	return nullptr;
}

void exd::Component::entity_add(Entity ent)
{
	u64 id = ent.id_extract();

	if (*sparse_ents.get_unsafe(id) != INVALID_ENTITY.id)
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to add an entity into this component, but it already exists. ID (no generation): %zu", ent.id_extract());
		return;
	}

	dense_ents.push(ent);
	sparse_ents.set_unsafe(id, dense_ents.back());

	//This originally moved the push_idx in the data container forward for iteration
	//Maybe unneeded? Kind of convenient though
	// data.push_without_data();

	//TODO(zshoals Dec-24-2022):> No entset added yet. Do we still need one?
	// internal_add_this_comp_to_entset(id);

	++active_entities;
}

void exd::Component::entity_remove(Entity ent)
{
	if (active_entities < 1)
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		return;
	}

	u64 id = ent.id_extract();

	size_t target_idx = *sparse_ents.get_unsafe(id);
	if (target_idx == INVALID_ENTITY.id) 
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		return;
	};

	Entity dense_ent = *dense_ents.get_unsafe(target_idx);

	bool valid = ent.matches(dense_ent);
	if (valid)
	{
		dense_ents.swap_and_pop(target_idx);

		//TODO(zshoals):>URGENT! FIXME!!!
		data.swap_and_pop(target_idx);

		Entity reciprocal = *dense_ents.get_unsafe(target_idx);

		//Note(zshoals Dec-19-2022):> Order is important here, if the element is the final one
		//in the array, reciprocal and ent id resolve to the same thing. So set INVALID_ENTITY.id
		//second
		sparse_ents.set_unsafe(reciprocal.id_extract(), target_idx);
		sparse_ents.set_unsafe(ent.id_extract(), INVALID_ENTITY.id);

		//TODO(zshoals):>URGENT! FIXME!!!
		internal_remove_this_comp_from_entset(id);

		--active_entities;
		}
	else
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
	}
}


void * exd::Component::set_untyped(Entity ent)
{
	//TODO(zshoals):> Wait, we need to make sure that we set up our arrays and stuff
	ENSURE_UNIMPLEMENTED();
	return calc_element_address(ent);
}

void exd::Component::clear(void)
{
	ENSURE_UNIMPLEMENTED();	
}
