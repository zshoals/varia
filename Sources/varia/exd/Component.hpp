#pragma once

#include "varia/ds/StaticArray.hpp"
#include "varia/validation.hpp"

#include "Entity.hpp"

namespace exd
{

//Component is a very thin data container. Due to cache coherency concerns,
//the bitset that corresponds to this container is not stored here.
template<typename T, int Size>
struct Component
{
	vds::StaticArray<T, Size> comps = {};
	u8 id_bits = 0;
	
	Component(u8 id_bits) : id_bits{id_bits} {}

	T const * comp_get(Entity ent) 
	{ 
		//TODO(zshoals): This must perform a check against the global entity storage
		//otherwise we can't confirm that the entities match in terms of generation
		//for iterable entities, we can use an unchecked version instead
		ENSURE_UNIMPLEMENTED();
		return comps.get(ent.id_extract(id_bits)); 
	} 

	T const * comp_get_unsafe(Entity ent) { ENSURE_UNIMPLEMENTED(); }

	T * comp_get_mut(Entity ent) 
	{ 
		//TODO(zshoals): This must perform a check against the global entity storage
		//otherwise we can't confirm that the entities match in terms of generation
		//for iterable entities, we can use an unchecked version instead
		ENSURE_UNIMPLEMENTED();
		return comps.get_mut(ent.id_extract(id_bits)); 
	}

	T * comp_get_mut_unsafe(Entity ent) { return comps.get_mut_unsafe(ent.id_extract(id_bits)); }
};

}