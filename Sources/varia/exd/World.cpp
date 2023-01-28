#include "World.hpp"
#include "Entity.hpp"
#include "EXDConstants.hpp"
#include "ComponentTypes.hpp"

#include "varia/ds/StaticArray.hpp"

#include "varia/logging.hpp"
#include "varia/util/Memory.hpp"

// exd::World::World(vds::Allocator * allocator) : allocator{allocator}
// {
// 	for_range_var(i, Constants::exd_max_entities)
// 	{
// 		manifest.get_mut(i)->id = i;
// 		freelist.push(Constants::exd_max_entities - i - 1);
// 	}
// }

static inline exd_component_t * exd_world_unitlocal_component_select(exd_world_t * world, exd::ComponentTypeID type)
{
	return &world->components[ComponentTypeID_to_raw(type)];
}

static inline exd_entity_t exd_world_unitlocal_manifest_get_modern_entity_copy(exd_world_t * world, exd_entity_t ent)
{
	return *(vds_array_get_unsafe(&world->manifest, exd_entity_id_extract(ent)));
}

static inline exd_entity_t * exd_world_unitlocal_manifest_get_modern_entity_reference(exd_world_t * world, exd_entity_t ent)
{
	return vds_array_get_mut_unsafe(&world->manifest, exd_entity_id_extract(ent));
}

void exd_world_initialize(exd_world_t * world, vds::Allocator * allocator)
{
	world->allocator = allocator;

	vds_array_initialize(&world->manifest);
	vds_array_initialize(&world->freelist);

	//Note(zshoals Dec-26-2022):> Components are registered on demand, so just zero 
	//to start with but do not actually register components
	VARIA_ZERO_INIT_SIZE(&world->components[0], sizeof(world->components));
	world->active_entities = 0;

	for_range_var(i, exd::Constants::exd_max_entities)
	{
		vds_array_get_mut(&world->manifest, i)->id = i;
		vds_array_push(&world->freelist, exd::Constants::exd_max_entities - i - 1);
	}
}

exd_entity_t exd_world_ent_create(exd_world_t * world)
{
	if (vds_array_is_empty(&world->freelist))
	{
		ENSURE_UNREACHABLE("Exhausted all available entities.");
	}

	++(world->active_entities);

	//TODO(zshoals 01-28-2023):> Why are we using ent_id here instead of an exd_entity_t? Why does the freelist not use
	//entities?
	u64 ent_id = vds_array_pop(&world->freelist);
	return *( vds_array_get(&world->manifest, ent_id) );
}

bool exd_world_ent_kill(exd_world_t * world, exd_entity_t ent)
{
	u64 ent_id = exd_entity_id_extract(ent);
	exd_entity_t * target_ent = exd_world_unitlocal_manifest_get_modern_entity_reference(world, ent);

	if (exd_entity_matches(ent, *target_ent))
	{
		--(world->active_entities);

		vds_array_push(&world->freelist, ent_id);
		exd_entity_generation_increment(target_ent);

		exd_internal_world_ent_remove_from_components(world, ent);

		return true;
	}
	else
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "EntityManifest tried to release a mismatched entity. exd_entity_t: %zu", ent.id);
		return false;
	}
}

bool exd_world_ent_valid(exd_world_t * world, exd_entity_t ent)
{
	exd_entity_t manifest_entity_revision = exd_world_unitlocal_manifest_get_modern_entity_copy(world, ent);
	return exd_entity_matches(ent, manifest_entity_revision);
}

//TODO(zshoals 01-01-2023):> Check for double registration? 
void exd_world_comp_register(exd_world_t * world, size_t element_size, exd::ComponentTypeID type)
{
	void * storage_mem = world->allocator->allocate_aligned_count(element_size, exd::Constants::exd_max_entities, Varia::Memory::default_alignment);

	//TODO(zshoals 01-28-2023):> Verify if this code is the same as the replacement in practice
	// size_t idx = ComponentTypeID_to_raw(type);
	// exd_component_t * comp = &world->components[idx];
	exd_component_t * comp = exd_world_unitlocal_component_select(world, type);
	exd_component_initialize(comp, storage_mem, element_size, exd::Constants::exd_max_entities, type);
}

void const * exd_world_comp_get(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type)
{

	return exd_component_get_untyped(exd_world_unitlocal_component_select(world, type), ent);
	
	//This is what it should be equivalent to; left in due to unclear-ness
	// return world->components[ComponentTypeID_to_raw(type)].get_untyped(ent);
}

void * exd_world_comp_get_mutable(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type)
{
	return exd_component_get_untyped_mutable(exd_world_unitlocal_component_select(world, type), ent);

	//This is what it should be equivalent to; left in due to unclear-ness
	// return world->components[ComponentTypeID_to_raw(type)].get_untyped_mutable(ent);
}

//TODO(zshoals 01-01-2023):> We aren't actually guaranteeing that this component exists before we set it
//We need a validation step of some sort across the entire interface before we access a component,
//at least for debugging where errors can happen easily
void * exd_world_comp_set(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type)
{
	exd_component_t * comp = exd_world_unitlocal_component_select(world, type);
	if (exd_world_ent_valid(world, ent))
	{
		//TODO(zshoals 01-07-2023):> We can add the component to the entity's entset here
		//we've modified entity_add to return true on a successful addition of that component
		//WARNING: Failure on duplicate entity component addition?
		bool was_added = exd_component_entity_add(comp, ent);
		return exd_component_get_untyped_mutable(comp, ent);
	}

	ENSURE_UNREACHABLE("ERROR: Tried to set a component with an out of date entity.");
	return nullptr;
}

bool exd_world_comp_remove(exd_world_t * world, exd_entity_t ent, exd::ComponentTypeID type)
{
	u64 ent_id = exd_entity_id_extract(ent);
	exd_entity_t * target_ent = exd_world_unitlocal_manifest_get_modern_entity_reference(world, ent);

	//Note(zshoals 01-01-2023):> This might be duplicated work
	//We kind of already check for entity validity in the component as well, but I think it's better off
	//checked here. Is the redundant check in component->entity_remove necessary? not sure
	//might not be possible for the component to go out of sync with the world's entities
	if (exd_entity_matches(ent, *target_ent))
	{
		exd_component_t * comp = exd_world_unitlocal_component_select(world, type);
		exd_component_entity_remove(comp, ent);

		return true;
	}
	else
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "EntityManifest tried to release a mismatched entity. Entity: %zu", ent.id);
		return false;
	}
}

exd_view_t exd_world_create_view(exd_world_t * world)
{
	exd_view_t v;
	exd_view_initialize(&v, world);
	return v;
}


void exd_internal_world_ent_remove_from_components(exd_world_t * world, exd_entity_t ent)
{
	//Note(zshoals Dec-27-2022):> We'll only complicate this further if dumb removal
	//becomes a serious time issue

	//Note(zshoals 01-28-2023):> Old version, keep around until verified working
	// world->components[ComponentTypeID_to_raw(ComponentTypeID::VARIA_CONCAT(TYPE, _e))].entity_remove(ent);

	#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME)\
	exd_component_entity_remove(&world->components[exd::ComponentTypeID_to_raw(exd::ComponentTypeID::VARIA_CONCAT(TYPE, _e))], ent);

	#include "ComponentData.def"
}
