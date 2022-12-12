#pragma once

#include "Entity.hpp"
#include "World.hpp"
#include "EXDUtil.hpp"
#include "EXDConstants.hpp"

#include "varia/ds/Bitset.hpp"
#include "varia/ds/StaticArray.hpp"
#include "varia/validation.hpp"

//Note(zshoals Dec-12-2022): Lame, but for convenient usage of components we have to 
//break this circular dependency, otherwise life is going to get annoying real fast
//Kind of bad architecture but we're interntionally doing weird things with the 
//component bitsets that are stored in World
template<int Size> struct World;

namespace exd
{

//Component is a very thin data container. Due to cache coherency concerns,
//the bitset that corresponds to this container is not stored here but rather in World
template<typename T, int Size>
struct Component
{
	vds::StaticArray<T, Size> comps = {};
	size_t bitset_handle = SIZE_MAX;
	constexpr static u8 id_bits = EXDUtil::id_shift(Size);

	T const * comp_get(World<Size> * w, Entity ent) 
	{ 
		vds::Bitset32<Size> * ent_states = w->internal_bitset_lookup(bitset_handle);
		u64 id = ent.id_extract(id_bits);
		bool alive = ent_states->is_set(id);
		bool valid = w->ent_valid(ent);

		if (alive && valid)
		{
			//Note(zshoals Dec-12-2022): We already know that the selected ID will be in bounds
			//as everything in World is templated to a fixed size
			return comps.get_unsafe(id);
		}
		else
		{
			return nullptr;
		}
	} 

	T * comp_get_mut(World<Size> * w, Entity ent) 
	{ 
		vds::Bitset32<Size> * ent_states = w->internal_bitset_lookup(bitset_handle);
		u64 id = ent.id_extract(id_bits);
		bool alive = ent_states->is_set(id);
		bool valid = w->ent_valid(ent);

		if (alive && valid)
		{
			//Note(zshoals Dec-12-2022): We already know that the selected ID will be in bounds
			//as everything in World is templated to a fixed size
			return comps.get_mut_unsafe(id);
		}
		else
		{
			return nullptr;
		}
	}

	T const * comp_get_unchecked(Entity ent) { return comps.get_unsafe(ent.id_extract(id_bits)); }
	T * comp_get_mut_unchecked(Entity ent) { return comps.get_mut_unsafe(ent.id_extract(id_bits)); }

	T * comp_set_ent(World<Size> * w, Entity ent)
	{
		vds::Bitset32<Size> * ent_states = w->internal_bitset_lookup(bitset_handle);
		u64 id = ent.id_extract(id_bits);
		bool not_set_in_component = ent_states->is_unset(id);
		bool valid = w->ent_valid(ent);

		DEBUG_ENSURE_TRUE(not_set_in_component, "Entity was already created in this bitset; did you set this entity twice?");
		DEBUG_ENSURE_TRUE(valid, "Entity was invalid while trying to set a component; did you use an expired entity?");

		if (not_set_in_component && valid)
		{
			//Note(zshoals Dec-12-2022): We already know that the selected ID will be in bounds
			//as everything in World is templated to a fixed size
			ent_states->set(id);
			return comps.get_mut_unsafe(id);
		}
		else
		{
			return nullptr;
		}
	}

	T * comp_set_ent_unchecked(Entity ent) { return comps.get_mut_unsafe(ent.id_extract(id_bits)); }

	bool comp_has(Entity ent)
	{
		vds::Bitset32<Size> * ent_states = w->internal_bitset_lookup(bitset_handle);
		u64 id = ent.id_extract(id_bits);
		bool alive = ent_states->is_set(id);
		bool valid = w->ent_valid(ent);

		return (alive && valid);
	}
};

}