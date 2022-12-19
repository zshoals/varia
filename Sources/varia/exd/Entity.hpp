#pragma once

#include "varia/vcommon.hpp"
#include "varia/exd/EXDConstants.hpp"
#include "varia/ds/Bits.hpp"

namespace exd
{

struct Entity
{
	u64 id;

	Entity(u64 value) : id{value} {}
	Entity() : id{0} {}

	inline void generation_increment(void)
	{
		vds::Bits64 bitmode = {};
		bitmode.storage = this->id;
		bitmode.increment_upper(Constants::exd_id_shift);
		this->id = bitmode.storage;
	}

	inline u64 generation_extract(void)
	{
		vds::Bits64 bitmode = {};
		bitmode.storage = this->id;
		bitmode.rotate_right(Constants::exd_id_shift + 1);

		return bitmode.storage;
	}

	inline u64 id_extract(void)
	{
		vds::Bits64 bitmode = {};
		bitmode.storage = this->id;

		vds::Bits64 lo_mask = {};
		lo_mask.generate_bitmask_lo(Constants::exd_id_shift);

		bitmode.mask_allow(lo_mask);

		return bitmode.storage;
	}

	inline bool matches(Entity other) { return this->id == other.id; }
};

static Entity const INVALID_ENTITY = {SIZE_MAX};

}