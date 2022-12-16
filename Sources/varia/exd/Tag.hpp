#pragma once

#include "varia/ds/Bitset.hpp"

#include "Entity.hpp"
#include "EXDUtil.hpp"
#include "World.hpp"

template<int Size> struct World;

namespace exd
{

template<int Size>
struct Tag
{
	vds::Bitset32<Size> entities;
	constexpr static u8 id_bits = EXDUtil::id_shift(Size);

	void tag_set(World<Size> * w, exd::Entity ent)
	{
		if (w->ent_valid(ent))
		{
			entities.set(ent.id_extract(id_bits));
		}
		else
		{
			ENSURE_UNREACHABLE("Error: Tried to set a tag with an invalid entity. Did your entity expire?");
		}
	}

	void tag_unset(World<Size> * w, exd::Entity ent)
	{
		if (w->ent_valid(ent))
		{
			entities.unset(ent.id_extract(id_bits));
		}
		else
		{
			//Note(zshoals Dec-12-2022):
			//No operation: It's acceptable to remove entities that are not actually set 
		}
	}

	bool tag_get(World<Size> * w, exd::Entity ent)
	{
		if (w->ent_valid(ent))
		{
			return entities.is_set(ent.id_extract(id_bits));
		}
		else
		{
			ENSURE_UNREACHABLE("Error: Tried to check a tag with an invalid entity. Did your entity expire?");
			return false;
		}
	}

	void tag_set_unchecked(exd::Entity ent)
	{
		entities.set(ent.id_extract(id_bits));
	}

	void tag_unset_unchecked(exd::Entity ent)
	{
		entities.unset(ent.id_extract(id_bits));
	}

	bool tag_get_unchecked(exd::Entity ent)
	{
		return entities.is_set(ent.id_extract(id_bits));
	}

};

}
