#pragma once

#include "Entity.hpp"
#include "EntityManifest.hpp"
#include "Component.hpp"
#include "varia/ds/Bitset.hpp"
#include "varia/math/Math.hpp"
#include "varia/vcommon.hpp"

#define EXD_IMPORT_COMPONENT_INCLUDES
#include "ComponentData.def"
#undef EXD_IMPORT_COMPONENT_INCLUDES

namespace exd
{

namespace WorldUtil
{
	constexpr u8 id_shift(size_t entity_count)
	{
		size_t nearest = Varia::Math::pow2_next_nearest(entity_count);
		u8 shift = Varia::Math::pow2_to_bitshift_value(nearest);

		//Note(zshoals Dec-12-2022): The shift value is used for bitmasking
		//This value should result in 
		return shift - 1;
	}
}

template<int Size>
struct World
{
	//TODO(zshoals): This struct needs to define a COMPONENT_DATA macro and import all components
	//And also bitsets and tags

	exd::EntityManifest<Size> entities;
	constexpr static u8 id_shift = WorldUtil::id_shift(Size);

	//====================================================
	//           Import all component bitsets
	//====================================================

	#define internal_bitset_name VARIA_CONCAT(internal_ents_, FIELD_NAME)

	//Note(zshoals Dec-11-2022): Disable warning 4005
	//Macro redefinition warning, however we are intentionally overwriting the macro here
	#pragma warning(push)
	#pragma warning(disable: 4005)

	#define COMPONENT_DATA(TYPE, FIELD_NAME) vds::Bitset32<Size> internal_bitset_name = {};
	#include "ComponentData.def"

	#pragma warning(pop)

	//====================================================
	//           Import all component data arrays
	//====================================================
	#pragma warning(push)
	#pragma warning(disable: 4005)

	#define COMPONENT_DATA(TYPE, FIELD_NAME) exd::Component<TYPE, Size> FIELD_NAME = {id_shift}; 
	#include "ComponentData.def"

	#pragma warning(pop)

	//====================================================
	//          End bitsets and data arrays 
	//====================================================

	exd::Entity ent_create(void) { return entities.entity_get_free(); }
	void ent_kill(exd::Entity ent) { entities.entity_release(ent, id_shift); internal_remove_from_components(ent); };
	void ent_valid(exd::Entity ent) { entities.entity_valid(ent, id_shift); }
	size_t ent_id(exd::Entity ent) { return ent.id_extract(id_shift); }
	size_t ent_gen(exd::Entity ent) { return ent.generation_extract(id_shift); }

	void internal_remove_from_components(exd::Entity ent)
	{

	//====================================================
	//   Generate removal functions for all components
	//====================================================
		#pragma warning(push)
		#pragma warning(disable: 4005)

		#define COMPONENT_DATA(TYPE, FIELD_NAME) internal_bitset_name.unset(ent.id_extract(id_shift));
		#include "ComponentData.def"

		#pragma warning(pop)
	}




	//Note: Might be unnecessary, check component array instead
	void ent_has(void) { ENSURE_UNIMPLEMENTED(); }; 
};

}