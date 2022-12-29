#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "EXDConstants.hpp"
#include "View.hpp"
#include "ComponentTypes.hpp"

#include "varia/ds/Allocator.hpp"

namespace exd
{

struct View;

struct World
{
	vds::Allocator * allocator;

	vds::StaticArray<Entity, Constants::exd_max_entities> manifest;
	vds::StaticArray<u64, Constants::exd_max_entities> freelist;
	//Note(zshoals Dec-26-2022):> We might also need to keep track if entities are alive or not
	//not sure if implicit is good enough

	Component components[exd::Constants::exd_max_components];

	size_t active_entities;




	// World(vds::Allocator * allocator);

	void initialize(vds::Allocator * allocator);
	Entity ent_create(void);
	bool ent_kill(Entity ent);
	bool ent_valid(Entity ent);

	void comp_register(size_t element_size, ComponentTypeID type);
	void const * comp_get(Entity ent, ComponentTypeID type);
	void * comp_get_mutable(Entity ent, ComponentTypeID type);
	void const * comp_get_unchecked(Entity ent, ComponentTypeID type);
	void * comp_get_mutable_unchecked(Entity ent, ComponentTypeID type);
	void * comp_set(Entity ent, ComponentTypeID type);

	exd::View view_create(void);


	private:
	void internal_ent_remove_from_components(Entity ent);
};

}