#pragma once

#include "ComponentTypes.hpp"
#include "Entity.hpp"
#include "varia/ds/StaticArray.hpp"
#include "varia/ds/Allocator.hpp"

struct exd_component_t
{
	vds_array_t<size_t, exd::Constants::exd_max_entities> sparse_ents;
	vds_array_t<exd_entity_t, exd::Constants::exd_max_entities> dense_ents;
	void * data;

	size_t element_count;
	size_t per_element_size;

	exd::ComponentTypeID UUID;
	size_t active_entities;

	//Debug information
	size_t memory_usage_in_bytes;
};

	// Component(void);
void exd_component_initialize(exd_component_t * comp, void * mem, size_t element_size, size_t element_count, exd::ComponentTypeID UUID);

size_t exd_component_length(exd_component_t * comp);

void const * exd_component_get_untyped(exd_component_t * comp, exd_entity_t ent);
void * exd_component_get_untyped_mutable(exd_component_t * comp, exd_entity_t ent);
void const * exd_component_get_untyped_unchecked(exd_component_t * comp, exd_entity_t ent);
void * exd_component_get_untyped_mutable_unchecked(exd_component_t * comp, exd_entity_t ent);
void const * exd_component_get_untyped_direct(exd_component_t * comp, size_t idx);
void * exd_component_get_untyped_mutable_direct(exd_component_t * comp, size_t idx);
bool exd_component_entity_add(exd_component_t * comp, exd_entity_t ent);
bool exd_component_entity_remove(exd_component_t * comp, exd_entity_t ent);
bool exd_component_has(exd_component_t * comp, exd_entity_t ent);
void exd_component_clear(exd_component_t * comp);

//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                   These should be private bs                        ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//These should be interna/"translation unit aka tunit"/static
void * exd_component_calc_element_address(exd_component_t * comp, exd_entity_t ent);

//TODO(zshoals 01-27-2023):> We don't really need to pass the active entity count as the index anymore
void * exd_component_calc_element_address_raw(exd_component_t * comp, size_t idx);

//TODO(zshoals 01-27-2023):> Did component need to be passed in here or just the data?
void exd_component_push_comp(exd_component_t * comp, void const * data);
void exd_component_push_comp_without_data(exd_component_t * comp);
void exd_component_swap_and_pop_comp(exd_component_t * comp, size_t idx);
size_t exd_component_back(exd_component_t * comp);
size_t exd_component_front(exd_component_t * comp);