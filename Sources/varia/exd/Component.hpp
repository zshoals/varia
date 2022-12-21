#pragma once

#include "varia/logging.hpp"
#include "varia/vcommon.hpp"
#include "varia/ds/StaticArray.hpp"
#include "EXDConstants.hpp"
#include "Entity.hpp"
#include "ComponentIdentifiers.hpp"

namespace exd
{

template<typename T>
struct Component
{
	vds::StaticArray<size_t, Constants::exd_max_entities> sparse_ents;
	vds::StaticArray<Entity, Constants::exd_max_entities> dense_ents;
	vds::StaticArray<T, Constants::exd_max_entities> data;

	//Note(zshoals Dec-21-2022):> This is gross, but keeps the interface cleaner...I guess.
	//This is pulled in from World. Entities have their own bitset indicating what components are attached
	//This is necessary so that killing an entity and removing it from all bitsets doesn't take 17 years
	vds::StaticArray<vds::Bitset32<Constants::exd_max_components>, Constants::exd_max_entities> * ent_comps_bitset;
	ComponentIdentifiers_e UID;

	size_t entity_count;

	Component
	(
		vds::StaticArray<vds::Bitset32<Constants::exd_max_components>, Constants::exd_max_entities> * ent_comps_bitset 
	)
	{
		entity_count = 0;
		sparse_ents.set_all(INVALID_ENTITY.id);
		this->ent_comps_bitset = ent_comps_bitset;
	}

	void internal_setUID(size_t UID)
	{
		this->UID = UID;
	}

	void internal_remove_this_comp_from_entset(u64 entity_id)
	{
		ent_comps_bitset->get_mut(entity_id)->unset(this->UID);
	}

	void internal_add_this_comp_to_entset(u64 entity_id)
	{
		ent_comps_bitset->get_mut(entity_id)->set(this->UID);
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
			return data.get_unsafe(target_idx);
		}
		ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

		return nullptr;
	}

	T * get_mut(Entity ent)
	{
		if (has(ent))
		{
			size_t target_idx = *sparse_ents.get_unsafe(ent.id_extract());
			return data.get_mut_unsafe(target_idx);
		}
		ENSURE_UNREACHABLE("This might be an error...should trying to get an ent that doesn't exist crash?");

		return nullptr;
	}


	void add(Entity ent)
	{
		u64 id = ent.id_extract();

		if (*sparse_ents.get_unsafe(id) != INVALID_ENTITY.id)
		{
			VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to add an entity into this component, but it already exists. ID (no generation): %zu", ent.id_extract());
			return;
		}

		dense_ents.push(ent);
		sparse_ents.set_unsafe(id, dense_ents.back());
		data.push_without_data();

		internal_add_this_comp_to_entset(id);

		++entity_count;
	}

	void remove(Entity ent)
	{
		if (entity_count < 1)
		{
			// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
			return;
		}

		u64 id = ent.id_extract();

		size_t target_idx = *sparse_ents.get_unsafe(id);
		if (target_idx == INVALID_ENTITY.id) 
		{
			// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
			return;
		};

		Entity dense_ent = *dense_ents.get_unsafe(target_idx);

		bool valid = ent.matches(dense_ent);
		if (valid)
		{
			dense_ents.swap_and_pop(target_idx);
			data.swap_and_pop(target_idx);
			Entity reciprocal = *dense_ents.get(target_idx);

			//Note(zshoals Dec-19-2022):> Order is important here, if the element is the final one
			//in the array, reciprocal and ent id resolve to the same thing. So set INVALID_ENTITY.id
			//second
			sparse_ents.set_unsafe(reciprocal.id_extract(), target_idx);
			sparse_ents.set_unsafe(ent.id_extract(), INVALID_ENTITY.id);

			internal_remove_this_comp_from_entset(id);

			--entity_count;
		}
		else
		{
			// VARIA_LOG(LOG_WARNING | LOG_ECS, "Tried to remove an entity that doesn't exist in this component. ID (no generation): %zu", ent.id_extract());
		}
	}

};

}