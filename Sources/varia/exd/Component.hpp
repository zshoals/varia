#pragma once

#include "Entity.hpp"
#include "varia/ds/StaticArray.hpp"
#include "varia/ds/Allocator.hpp"

namespace exd
{

struct Component
{
	void * data;
	size_t push_idx;

	size_t element_count;
	size_t per_element_size;

	size_t UUID;
	size_t active_entities;

	vds::StaticArray<size_t, Constants::exd_max_entities> sparse_ents;
	vds::StaticArray<Entity, Constants::exd_max_entities> dense_ents;



	Component(void);
	void initialize(void * mem, size_t element_size, size_t element_count, size_t UUID);

	void const * get_untyped(Entity ent);
	void * get_untyped_mutable(Entity ent);
	void entity_add(Entity ent);
	void entity_remove(Entity ent);
	bool has(Entity ent);
	void clear(void);

	private:
	void * calc_element_address(Entity ent);
	void * calc_element_address_raw(size_t idx);
	void push_comp(void const * data);
	void push_comp_without_data(void);
	void pop_and_swap_comp(size_t idx);
	size_t back(void);
	size_t front(void);
};

}