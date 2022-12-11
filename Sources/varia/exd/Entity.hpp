#pragma once

#include "varia/vcommon.hpp"
#include "varia/ds/Bits.hpp"

namespace exd
{

struct Entity
{
	u64 id;

	inline void generation_increment(u8 generation_pivot)
	{
		vds::Bits64 bitmode = {};
		bitmode.storage = this->id;
		bitmode.increment_upper(generation_pivot);
		this->id = bitmode.storage;
	}

	inline u64 generation_extract(u8 generation_pivot)
	{
		vds::Bits64 bitmode = {};
		bitmode.storage = this->id;
		bitmode.rotate_right(generation_pivot);

		return bitmode.storage;
	}

	inline u64 id_extract(u8 generation_pivot)
	{
		vds::Bits64 bitmode = {};
		bitmode.storage = this->id;

		vds::Bits64 lo_mask = {};
		lo_mask.generate_bitmask_lo(generation_pivot);

		bitmode.mask_allow(lo_mask);

		return bitmode.storage;
	}

	inline bool matches(Entity other) { return this->id == other.id; }
};

constexpr Entity INVALID_ENTITY = {};

}