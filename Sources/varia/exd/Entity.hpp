#pragma once

#include "varia/vcommon.hpp"
#include "varia/ds/Bits.hpp"

namespace exd
{

struct Entity
{
	u64 id;

	inline void increment_generation(u8 generation_pivot)
	{
		vds::Bits64 id = {};
		id.storage = this->id;
		id.increment_upper(generation_pivot);
	}

	inline u64 extract_id_exclusively(u8 generation_pivot)
	{
		vds::Bits64 id = {};
		vds::Bits64 lo_mask = {};
		lo_mask.not();
		lo_mask.generate_bitmask_lo(generation_pivot);

		id.mask_allow(lo_mask);

		return id.storage;
	}

	inline bool matches(Entity other) { return this->id == other.id; }
};

constexpr Entity INVALID_ENTITY = {};

}