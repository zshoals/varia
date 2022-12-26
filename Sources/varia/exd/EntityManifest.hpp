#pragma once

#include "varia/ds/Bitset.hpp"
#include "varia/ds/Result.hpp"
#include "varia/ds/StaticArray.hpp"
#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "varia/logging.hpp"
#include "EXDConstants.hpp"
#include "Entity.hpp"

namespace exd
{

struct EntityManifest
{
	vds::StaticArray<Entity, Constants::exd_max_entities> manifest;
	vds::StaticArray<u64, Constants::exd_max_entities> freelist;
	// vds::StaticArray<exd::Entity, 10> manifest;

	EntityManifest(void)
	{
		for_range_var(i, Constants::exd_max_entities)
		{
			manifest.get_mut(i)->id = i;
			freelist.push(Constants::exd_max_entities - i - 1);
		}
	}

	bool valid(Entity ent)
	{
		//Note(zshoals Dec-25-2022):>
		//Isn't this supposed to be ent.id_extract? 
		return ent.matches(*manifest.get(ent.id));
	}
	
	Entity get_free(void)
	{
		if (freelist.is_empty())
		{
			ENSURE_UNREACHABLE("Exhausted all available entities.");
		}

		u64 ent_id = freelist.pop();
		return *manifest.get(ent_id);
	}

	bool release(Entity ent)
	{
		u64 ent_id = ent.id_extract();
		Entity * target_ent = manifest.get_mut(ent_id);

		if (ent.matches(*target_ent))
		{
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

};

}