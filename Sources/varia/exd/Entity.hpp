#pragma once

#include "varia/vcommon.hpp"
#include "varia/exd/EXDConstants.hpp"
#include "varia/ds/Bits.hpp"


struct exd_entity_t
{
	u64 id;
};

//TODO(zshoals 01-28-2023):> Uh oh...where were these used in the codebase?
// Entity(u64 value) : id{value} {}
// Entity() : id{0} {}





//TODO(zshoals 01-28-2023):> Needs massive rework...requires pointers everywhere or return values
//TODO(zshoals 01-28-2023):> Rename all "this" equivalent parameters to "self"
inline void exd_entity_generation_increment(exd_entity_t * ent)
{
	//TODO(zshoals 01-28-2023):> God damnit, bits need reworked too :/
	vds::Bits64 bitmode = {};
	bitmode.storage = ent->id;
	bitmode.increment_upper(EXD_CONSTANTS_ID_SHIFT);
	ent->id = bitmode.storage;
}

inline u64 exd_entity_generation_extract(exd_entity_t ent)
{
	vds::Bits64 bitmode = {};
	bitmode.storage = ent.id;
	bitmode.rotate_right(EXD_CONSTANTS_ID_SHIFT);

	return bitmode.storage;
}

inline u64 exd_entity_id_extract(exd_entity_t ent)
{
	vds::Bits64 bitmode = {};
	bitmode.storage = ent.id;

	vds::Bits64 lo_mask = {};
	lo_mask.generate_bitmask_lo(EXD_CONSTANTS_ID_SHIFT);

	bitmode.mask_allow(lo_mask);

	return bitmode.storage;
}

inline bool exd_entity_matches(exd_entity_t a, exd_entity_t b) 
{ 
	return a.id == b.id;
}

inline bool exd_entity_matches_or_exceeds(exd_entity_t a, exd_entity_t b)
{
	bool genOK = ( exd_entity_generation_extract(a) >= exd_entity_generation_extract(b) );
	bool idOK = ( exd_entity_id_extract(a) == exd_entity_id_extract(b) );

	return (genOK && idOK);
}

//TODO(zshoals 01-28-2023):> Temporary invalid entity location to keep things working
namespace exd
{
	static exd_entity_t const INVALID_ENTITY = {SIZE_MAX};
}