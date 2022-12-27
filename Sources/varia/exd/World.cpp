#include "World.hpp"
#include "Entity.hpp"
#include "EXDConstants.hpp"

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
	VARIA_ZERO_INIT_SIZE(&this->components, sizeof(this->components));
	this->UUID_generator = 0;
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

void exd::World::comp_register(size_t element_size)
{
	DEBUG_ENSURE_UINT_LT(this->UUID_generator, Constants::exd_max_components, "No component slots available to register a new component.");

	void * storage_mem = this->allocator->allocate_aligned_count(element_size, exd::Constants::exd_max_entities, Varia::Memory::default_alignment);
	size_t current_slot = this->UUID_generator;
	++this->UUID_generator;

	Component * comp = &this->components[current_slot];
	comp->initialize(storage_mem, element_size, exd::Constants::exd_max_entities, current_slot);
}

void exd::World::internal_ent_remove_from_components(exd::Entity ent)
{
	ENSURE_UNIMPLEMENTED();
}
