#pragma once

#include "varia/logging.hpp"
#include "varia/vcommon.hpp"
#include "varia/ds/StaticArray.hpp"
#include "EXDConstants.hpp"
#include "Entity.hpp"

namespace exd
{

template<typename T>
struct Component
{
	vds::StaticArray<size_t, Constants::exd_max_entities> sparse_ents;
	vds::StaticArray<Entity, Constants::exd_max_entities> dense_ents;
	vds::StaticArray<T, Constants::exd_max_entities> data;
	size_t entity_count;

	Component(void)
	{
		VARIA_ZERO_INIT(this);
	}

	bool has(Entity ent)
	{
		u64 id = ent.id_extract();
		size_t target_idx = *sparse_ents.get_unsafe(id);

		if (target_idx != INVALID_ENTITY.id)
		{
			Entity other = *dense_ents.get_unsafe(target_idx);
			return ent.matches(other);
		}

		return false;
	}

	T const * get(Entity ent)
	{
		if (has(ent))
		{
			size_t target_idx = *sparse_ents.get_unsafe(ent.id_extract());
			Entity target_ent = *dense_ents.get_unsafe(target_idx);
			return data.get_unsafe(target_ent.id_extract());
		}
		ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

		return nullptr;
	}

	T * get_mut(Entity ent)
	{
		if (has(ent))
		{
			size_t target_idx = *sparse_ents.get_unsafe(ent.id_extract());
			Entity target_ent = *dense_ents.get_unsafe(target_idx);
			return data.get_mut_unsafe(target_ent.id_extract());
		}
		ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

		return nullptr;
	}


	void add(Entity ent)
	{
		u64 id = ent.id_extract();

		if (*sparse_ents.get_unsafe(id) == INVALID_ENTITY.id)
		{
			VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to push an entity into this component, but it already exists. ID (no generation): %zu", ent.id_extract());
			return;
		}

		dense_ents.push(ent);
		sparse_ents.set_unsafe(id, dense_ents.back());
		data.push_without_data();
		++entity_count;
	}

	void remove(Entity ent)
	{
		size_t target_idx = *sparse_ents.get_unsafe(ent.id_extract());
		if (target_idx == INVALID_ENTITY.id) return;

		Entity dense_ent = *dense_ents.get_unsafe(target_idx);
		bool valid = ent.matches(dense_ent);
		if (valid)
		{
			dense_ents.swap_and_pop(target_idx);
			data.swap_and_pop(target_idx);
			sparse_ents.set_unsafe(target_idx, INVALID_ENTITY.id);
			--entity_count;
		}
		else
		{
			VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		}
	}

};

}