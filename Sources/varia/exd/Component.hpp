#pragma once

#include "ComponentTypes.hpp"
#include "Entity.hpp"
#include "varia/ds/StaticArray.hpp"
#include "varia/ds/Allocator.hpp"

namespace exd
{

struct Component
{
	void * data;

	size_t element_count;
	size_t per_element_size;

	ComponentTypeID UUID;
	size_t active_entities;

	vds::StaticArray<size_t, Constants::exd_max_entities> sparse_ents;
	vds::StaticArray<Entity, Constants::exd_max_entities> dense_ents;

	//Debug information
	size_t memory_usage_in_bytes;




	// Component(void);
	void initialize(void * mem, size_t element_size, size_t element_count, ComponentTypeID UUID);

	size_t length(void);

	void const * get_untyped(Entity ent);
	void * get_untyped_mutable(Entity ent);
	void const * get_untyped_unchecked(Entity ent);
	void * get_untyped_mutable_unchecked(Entity ent);
	void const * get_untyped_direct(size_t idx);
	void * get_untyped_mutable_direct(size_t idx);
	void entity_add(Entity ent);
	bool entity_remove(Entity ent);
	bool has(Entity ent);
	void clear(void);

	private:
	void * calc_element_address(Entity ent);
	void * calc_element_address_raw(size_t idx);
	void push_comp(void const * data);
	void push_comp_without_data(void);
	void swap_and_pop_comp(size_t idx);
	size_t back(void);
	size_t front(void);
};

}