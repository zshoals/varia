#pragma once

#include "Entity.hpp"
#include "Component.hpp"
#include "ComponentTypes.hpp"

#include "varia/vcommon.hpp"
#include "varia/ds/StaticArray.hpp"

namespace exd
{

struct View
{
	vds::StaticArray<Component *, 8> comp_include;
	vds::StaticArray<ComponentTypeID, 8> comp_type_indices;
	vds::StaticArray<Component *, 8> comp_exclude;
	Component * shortest_dataset;

	bool finalized;




	void initialize(void);

	void include(Component * comp);
	void exclude(Component * comp);
	void compile(void);

	void const * comp_get(Entity ent, ComponentTypeID type);
	void * comp_get_mutable(Entity ent, ComponentTypeID type);

	#define ITERATE_VIEW(VIEW_PTR, PROCESSOR_FUNC)\
		do\
		{\
			assert((VIEW_PTR)->finalized);\
			size_t const len = (VIEW_PTR)->shortest_dataset->dense_ents.length();\
			for_range_var(i, len)\
			{\
				Entity ent = *(VIEW_PTR)->shortest_dataset->dense_ents.get_unsafe(i);\
				if ((VIEW_PTR)->internal_target_matches_query(ent))\
				{\
					PROCESSOR_FUNC((VIEW_PTR), ent);\
				}\
			}\
		}\
		while (0)
			
	bool internal_target_matches_query(Entity ent);

};

}