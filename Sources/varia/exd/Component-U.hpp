#pragma once

#include "Entity.hpp"
#include "varia/ds/StaticArray.hpp"

//Forwards
struct vds::Allocator;

namespace exd
{

struct Component
{
	vds::Allocator * arena;
	void * data;
	const size_t element_count;
	const size_t per_element_size;

	size_t UUID;
	size_t active_entities;

	vds::StaticArray<size_t, Constants::exd_max_entities> sparse_ents;
	vds::StaticArray<Entity, Constants::exd_max_entities> dense_ents;



	Component(vds::Allocator * mem, size_t element_size, size_t element_count);
	void const * get_untyped(Entity ent);
	void * get_untyped_mutable(Entity ent);
	void * set_untyped(Entity ent);
	void entity_add(Entity ent);
	void entity_remove(Entity ent);
	bool has(Entity ent);
	void clear(void);

	private:
	void * calc_element_address(Entity ent);
	void * calc_element_address_raw(size_t idx);
};

}