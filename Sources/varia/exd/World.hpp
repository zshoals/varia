#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "EXDConstants.hpp"
#include "View.hpp"
#include "ComponentTypes.hpp"

#include "varia/ds/Allocator.hpp"
#include "varia/ds/StaticArray.hpp"

struct exd_view_t;

struct exd_world_t
{
	vds::Allocator * allocator;

	vds_array_t<exd_entity_t, exd::Constants::exd_max_entities> manifest;
	vds_array_t<u64, exd::Constants::exd_max_entities> freelist;
	exd_component_t components[exd::Constants::exd_max_components];

	size_t active_entities;
};


//Note(zshoals Dec-26-2022):> We might also need to keep track if entities are alive or not
//not sure if implicit is good enough

// World(vds::Allocator * allocator);

void exd_world_initialize(exd_world_t * world, vds::Allocator * allocator);
exd_entity_t exd_world_ent_create(exd_world_t * world);
bool exd_world_ent_kill(exd_world_t * world, exd_entity_t ent);
bool exd_world_ent_valid(exd_world_t * world, exd_entity_t ent);

void exd_world_comp_register(exd_world_t * world, size_t element_size, exd::ComponentTypeID type);
void const * exd_world_comp_get(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type);
void * exd_world_comp_get_mutable(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type);
// void const * exd_world_comp_get_unchecked(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type);
// void * exd_world_comp_get_mutable_unchecked(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type);
void * exd_world_comp_set(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type);
bool exd_world_comp_remove(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type);

exd::View exd_world_create_view(void);

void exd_internal_world_ent_remove_from_components(exd_world_t * world, exd_entity_t ent);