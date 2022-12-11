#pragma once

#include "Entity.hpp"
#include "varia/ds/Bitset.hpp"
#include "varia/ds/StaticArray.hpp"
#include "varia/vcommon.hpp"
#include "varia/validation.hpp"

namespace exd
{

template <int Size>
struct EntityManifest
{
	vds::StaticArray<Entity, Size> manifest = {};
	//vds::Freelist<Size> freelist = {};
	vds::Bitset32<Size> bitset = {};

	Entset()
	{
		for_range_var(i, Size)
		{
			manifest.get_mut(i)->id = i;
		}
	}

	bool entity_valid(exd::Entity ent, u8 generation_pivot)
	{
		u64 idx = ent.extract_id_exclusively(generation_pivot);
		return (ent.id == this->manifest[idx].id);
	}

	void ent_get_free(void)
	{
		//Find first free...
		ENSURE_UNIMPLEMENTED();

	}

};

}