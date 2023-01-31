#pragma once

#include "varia/vcommon.hpp"
#include "varia/exd/EXDConstants.hpp"
#include "varia/ds/Bits.hpp"


struct exd_entity_t
{
	u64 id;
};


exd_entity_t constexpr EXD_ENTITY_INVALID_ENTITY = {SIZE_MAX};


//TODO(zshoals 01-28-2023):> Needs massive rework...requires pointers everywhere or return values
//TODO(zshoals 01-28-2023):> Rename all "this" equivalent parameters to "self"
inline void exd_entity_increment_generation(exd_entity_t * ent)
{
	ent->id = vds_bits64_increment_upper(ent->id, EXD_GLOBAL_CONSTANTS_ID_SHIFT);
}

inline u64 exd_entity_extract_generation(exd_entity_t ent)
{
	return vds_bits64_rotate_right(ent.id, EXD_GLOBAL_CONSTANTS_ID_SHIFT);
}

inline u64 exd_entity_extract_id(exd_entity_t ent)
{
	return vds_bits64_mask_allow(ent.id, vds_bits64_generate_bitmask_lo(EXD_GLOBAL_CONSTANTS_ID_SHIFT));
}

inline bool exd_entity_matches(exd_entity_t a, exd_entity_t b) 
{ 
	return a.id == b.id;
}

inline bool exd_entity_matches_or_exceeds(exd_entity_t a, exd_entity_t b)
{
	bool genOK = ( exd_entity_extract_generation(a) >= exd_entity_extract_generation(b) );
	bool idOK = ( exd_entity_extract_id(a) == exd_entity_extract_id(b) );

	return (genOK && idOK);
}
