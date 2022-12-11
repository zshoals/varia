#pragma once

#include "Entity.hpp"
#include "varia/ds/Bitset.hpp"
#include "varia/ds/Result.hpp"
#include "varia/ds/StaticArray.hpp"
#include "varia/vcommon.hpp"
#include "varia/validation.hpp"

namespace exd
{

template <int Size>
struct EntityManifest
{
	vds::StaticArray<Entity, Size> manifest;
	// vds::StaticArray<exd::Entity, 10> manifest;
	vds::Bitset32<Size> bitset = {};

	EntityManifest(void)
	{
		for_range_var(i, Size)
		{
			manifest.get_mut(i)->id = i;
		}

		//Entity 0 is an INVALID_ENTITY. It is always set.
		//Some special care has to be used when working with slot 0 of the manifest and bitset
		bitset.set(0);
	}

	bool entity_valid(exd::Entity ent, u8 generation_pivot)
	{
		u64 idx = ent.id_extract(generation_pivot);
		return (ent.id == this->manifest[idx].id);
	}

	exd::Entity entity_get_free(void)
	{
		vds::Result<size_t> idx = bitset.find_first_unset();
		if (idx.valid)
		{
			bitset.set(idx.value);
			return manifest.data[idx.value];
		}

		//Out of entities...
		//Should we try and handle this more gracefully or what?
		ENSURE_UNREACHABLE("Depleted all available entities.");
		return INVALID_ENTITY;
	}

	void entity_release(exd::Entity ent, u8 generation_pivot)
	{
		DEBUG_ENSURE_UINT_GT_ZERO(ent.id, "Tried to release an INVALID_ENTITY");
		DEBUG_ENSURE_UINT_LT(ent.id_extract(generation_pivot), Size, "Tried to release an entity id exceeding the maximum entity count.");

		size_t idx = ent.id_extract(generation_pivot);
		bitset.unset(idx);
		manifest.data[idx].generation_increment(generation_pivot);
	}

};

}