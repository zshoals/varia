#include "World.hpp"
#include "Entity.hpp"
#include "EXDConstants.hpp"
#include "ComponentTypes.hpp"

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

void exd::World::initialize(vds::Allocator * allocator)
{
	this->allocator = allocator;

	this->manifest.initialize();
	this->freelist.initialize();

	//Note(zshoals Dec-26-2022):> Components are registered on demand, so just zero 
	//to start with but do not actually register components
	VARIA_ZERO_INIT_SIZE(&this->components[0], sizeof(this->components));
	this->active_entities = 0;

	for_range_var(i, Constants::exd_max_entities)
	{
		manifest.get_mut(i)->id = i;
		freelist.push(Constants::exd_max_entities - i - 1);
	}
}

exd::Entity exd::World::ent_create(void)
{
	if (freelist.is_empty())
	{
		ENSURE_UNREACHABLE("Exhausted all available entities.");
	}

	++active_entities;

	u64 ent_id = freelist.pop();
	return *manifest.get(ent_id);
}

bool exd::World::ent_kill(exd::Entity ent)
{
	u64 ent_id = ent.id_extract();
	Entity * target_ent = manifest.get_mut(ent_id);

	if (ent.matches(*target_ent))
	{
		--active_entities;

		freelist.push(ent_id);
		target_ent->generation_increment();

		internal_ent_remove_from_components(ent);

		return true;
	}
	else
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "EntityManifest tried to release a mismatched entity. Entity: %zu", ent.id);
		return false;
	}
}

bool exd::World::ent_valid(exd::Entity ent)
{
	return ent.matches(*manifest.get(ent.id_extract()));
}

void exd::World::comp_register(size_t element_size, ComponentTypeID type)
{
	void * storage_mem = this->allocator->allocate_aligned_count(element_size, exd::Constants::exd_max_entities, Varia::Memory::default_alignment);

	size_t idx = ComponentTypeID_to_raw(type);
	Component * comp = &this->components[idx];
	comp->initialize(storage_mem, element_size, exd::Constants::exd_max_entities, type);
}

void const * exd::World::comp_get(Entity ent, ComponentTypeID type)
{
	return components[ComponentTypeID_to_raw(type)].get_untyped(ent);
}

void * exd::World::comp_get_mutable(Entity ent, ComponentTypeID type)
{
	return components[ComponentTypeID_to_raw(type)].get_untyped_mutable(ent);
}

void * exd::World::comp_set(Entity ent, ComponentTypeID type)
{
	Component * comp = &components[ComponentTypeID_to_raw(type)];
	if (ent.matches(*manifest.get_unsafe(ent.id_extract())))
	{
		comp->entity_add(ent);
		return comp->get_untyped_mutable(ent);
	}

	ENSURE_UNREACHABLE("For now, let's stop duplicate setting of components. This might be changed in the future.");
	return nullptr;
}

bool exd::World::comp_remove(exd::Entity ent, ComponentTypeID type)
{
	u64 ent_id = ent.id_extract();
	Entity * target_ent = manifest.get_mut(ent_id);

	//Note(zshoals 01-01-2023):> This might be duplicated work
	//We kind of already check for entity validity in the component as well, but I think it's better off
	//checked here. Is the redundant check in component->entity_remove necessary? not sure
	//might not be possible for the component to go out of sync with the world's entities
	if (ent.matches(*target_ent))
	{
		Component * comp = &this->components[ComponentTypeID_to_raw(type)];
		comp->entity_remove(ent);

		return true;
	}
	else
	{
		VARIA_LOG(LOG_WARNING | LOG_ECS, "EntityManifest tried to release a mismatched entity. Entity: %zu", ent.id);
		return false;
	}
}

exd::View exd::World::view_create(void)
{
	exd::View v;
	v.initialize(this);
	return v;
}


void exd::World::internal_ent_remove_from_components(exd::Entity ent)
{
	//Note(zshoals Dec-27-2022):> We'll only complicate this further if dumb removal
	//becomes a serious time issue
	#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME)\
	components[ComponentTypeID_to_raw(ComponentTypeID::VARIA_CONCAT(TYPE, _e))].entity_remove(ent);
	#include "ComponentData.def"
}
