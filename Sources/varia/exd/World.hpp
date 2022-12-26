#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "EXDConstants.hpp"

#include "varia/ds/Allocator.hpp"

namespace exd
{

struct World
{
	vds::Allocator * allocator;

	vds::StaticArray<Entity, Constants::exd_max_entities> manifest;
	vds::StaticArray<u64, Constants::exd_max_entities> freelist;

	Component components[exd::Constants::exd_max_components];

	size_t UUID_generator = 0;
	size_t active_entities = 0;




	World(vds::Allocator * allocator);

	Entity ent_create(void);
	bool ent_kill(Entity ent);
	bool ent_valid(Entity ent);

	void comp_register(size_t element_size);

	private:
	void internal_ent_remove_from_components(Entity ent);
};

}