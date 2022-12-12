#pragma once

#include "Entity.hpp"
#include "EntityManifest.hpp"
#include "Tag.hpp"
#include "Component.hpp"
#include "EXDUtil.hpp"
#include "EXDConstants.hpp"
#include "varia/ds/Bitset.hpp"
#include "varia/math/Math.hpp"
#include "varia/vcommon.hpp"

#define EXD_IMPORT_COMPONENT_INCLUDES
#include "ComponentData.def"
#undef EXD_IMPORT_COMPONENT_INCLUDES

namespace exd
{

template<int Size>
struct World
{
	//TODO(zshoals): This struct needs to define a COMPONENT_DATA macro and import all components
	//And also bitsets and tags

	exd::EntityManifest<Size> entities = {};
	constexpr static u8 id_shift = EXDUtil::id_shift(Size);
	u64 active_entities = 0;
	//Note(zshoals Dec-12-2022): We just set an upper bound here...hacky but simple...
	//500kb at 8192 entities at 512 bitsets
	vds::Bitset32<Size> component_ents[exd::Constants::exd_max_components] = {};
	//====================================================
	//           Import all tags, and then
	//           all component data
	//====================================================

	//Note(zshoals Dec-11-2022): Disable warning 4005
	//Macro redefinition warning, however we are intentionally overwriting the macro here
	#pragma warning(push)
	#pragma warning(disable: 4005)

	#define EXD_TAG(FIELD_NAME) exd::Tag<Size> FIELD_NAME = {};
	#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) exd::Component<TYPE, Size> FIELD_NAME = {id_shift}; 
	#include "ComponentData.def"

	#pragma warning(pop)

	//====================================================
	//          End tags and data arrays 
	//====================================================

	World(void)
	{
		//====================================================
		//          Set all component bitset handles
		//====================================================
		size_t counter = 0;

		#pragma warning(push)
		#pragma warning(disable: 4005)

		#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) FIELD_NAME.bitset_handle = counter; ++counter; 
		#include "ComponentData.def"

		#pragma warning(pop)

		DEBUG_ENSURE_UINT_LT(counter, exd::Constants::exd_max_components, "Exceeded max allowed components; raise the limit if you need more.");
	}


	exd::Entity ent_create(void) { ++active_entities; return entities.entity_get_free(); }
	void ent_kill(exd::Entity ent) { --active_entities; entities.entity_release(ent, id_shift); internal_remove_from_components(ent); };

	bool ent_valid(exd::Entity ent) { return entities.entity_valid(ent, id_shift); }

	size_t ent_id(exd::Entity ent) { return ent.id_extract(id_shift); }
	size_t ent_generation(exd::Entity ent) { return ent.generation_extract(id_shift); }

	void internal_remove_from_components(exd::Entity ent)
	{

		//====================================================
		//   Generate removal functions for all components
		//====================================================
		#pragma warning(push)
		#pragma warning(disable: 4005)

		size_t counter = 0;

		#define EXD_TAG(FIELD_NAME) FIELD_NAME.tag_unset(this, ent);
		#define EXD_COMPONENT_DATA(TYPE, FIELD_NAME) component_ents[counter].unset(ent.id_extract(id_shift)); ++counter;
		#include "ComponentData.def"

		#pragma warning(pop)
	}

	vds::Bitset32<Size> * internal_bitset_lookup(size_t idx)
	{
		DEBUG_ENSURE_UINT_LT(idx, exd::Constants::exd_max_components, "Overflow of range in component bitset selection.");
		return &component_ents[idx];
	}

};

}