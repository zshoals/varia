#pragma once
#include "varia/math/Math.hpp"
#include "Entity.hpp"
#include "Component.hpp"

namespace exd
{

namespace Helpers
{

typedef void (*system_callback)(Entity ent);

template<typename T>
size_t shortest_length(Component<T> * comp)
{
	return comp->entity_count;
}

template<typename T, typename U>
size_t shortest_length(Component<T> * comp1, Component<U> * comp2)
{
	return Varia::Math::minST(comp1->entity_count, comp2->entity_count);
}

template<typename T>
size_t shortest_length(Component<T> * comp1, Component<T> * comp2)
{
	return Varia::Math::minST(comp1->entity_count, comp2->entity_count);
}

template<typename FIRST, typename... REST>
size_t shortest_length(Component<FIRST> * first, Component<REST> * ...rest)
{
	return Varia::Math::minST(first->entity_count, shortest_length(rest...));
}


//Omfg

#define EXD_ITER_ELIF_8(FUNC, A, B, C, D, E, F, G, H)\
else if ((A)->entity_count <= (B)->entity_count && (A)->entity_count <= (C)->entity_count && (A)->entity_count <= (D)->entity_count && (A)->entity_count <= (E)->entity_count && (A)->entity_count <= (F)->entity_count && (A)->entity_count <= (G)->entity_count && (A)->entity_count <= (H)->entity_count)\
{\
	for (exd::Entity const & ent : (A)->dense_ents)\
	{\
		if ((B)->has(ent) && (C)->has(ent) && (D)->has(ent) && (E)->has(ent) && (F)->has(ent) && (G)->has(ent) && (H)->has(ent))\
		{\
			FUNC(ent);\
		}\
	}\
}

#define EXD_ITER_ELIF_7(FUNC, A, B, C, D, E, F, G)\
else if ((A)->entity_count <= (B)->entity_count && (A)->entity_count <= (C)->entity_count && (A)->entity_count <= (D)->entity_count && (A)->entity_count <= (E)->entity_count && (A)->entity_count <= (F)->entity_count && (A)->entity_count <= (G)->entity_count)\
{\
	for (exd::Entity const & ent : (A)->dense_ents)\
	{\
		if ((B)->has(ent) && (C)->has(ent) && (D)->has(ent) && (E)->has(ent) && (F)->has(ent) && (G)->has(ent))\
		{\
			FUNC(ent);\
		}\
	}\
}

#define EXD_ITER_ELIF_6(FUNC, A, B, C, D, E, F)\
else if ((A)->entity_count <= (B)->entity_count && (A)->entity_count <= (C)->entity_count && (A)->entity_count <= (D)->entity_count && (A)->entity_count <= (E)->entity_count && (A)->entity_count <= (F)->entity_count)\
{\
	for (exd::Entity const & ent : (A)->dense_ents)\
	{\
		if ((B)->has(ent) && (C)->has(ent) && (D)->has(ent) && (E)->has(ent) && (F)->has(ent))\
		{\
			FUNC(ent);\
		}\
	}\
}

#define EXD_ITER_ELIF_5(FUNC, A, B, C, D, E)\
else if ((A)->entity_count <= (B)->entity_count && (A)->entity_count <= (C)->entity_count && (A)->entity_count <= (D)->entity_count && (A)->entity_count <= (E)->entity_count)\
{\
	for (exd::Entity const & ent : (A)->dense_ents)\
	{\
		if ((B)->has(ent) && (C)->has(ent) && (D)->has(ent) && (E)->has(ent))\
		{\
			FUNC(ent);\
		}\
	}\
}

#define EXD_ITER_ELIF_4(FUNC, A, B, C, D)\
else if ((A)->entity_count <= (B)->entity_count && (A)->entity_count <= (C)->entity_count && (A)->entity_count <= (D)->entity_count)\
{\
	for (exd::Entity const & ent : (A)->dense_ents)\
	{\
		if ((B)->has(ent) && (C)->has(ent) && (D)->has(ent))\
		{\
			FUNC(ent);\
		}\
	}\
}

#define EXD_ITER_ELIF_3(FUNC, A, B, C)\
else if ((A)->entity_count <= (B)->entity_count && (A)->entity_count <= (C)->entity_count)\
{\
	for (exd::Entity const & ent : (A)->dense_ents)\
	{\
		if ((B)->has(ent) && (C)->has(ent))\
		{\
			FUNC(ent);\
		}\
	}\
}

#define EXD_ITER_ELIF_2(FUNC, A, B)\
else if ((A)->entity_count <= (B)->entity_count)\
{\
	for (exd::Entity const & ent : (A)->dense_ents)\
	{\
		if ((B)->has(ent))\
		{\
			FUNC(ent);\
		}\
	}\
}

#define EXD_ITER_8(FUNC, A, B, C, D, E, F, G, H)\
	EXD_USELESS_IF()\
	EXD_ITER_ELIF_8(FUNC, A, B, C, D, E, F, G, H)\
	EXD_ITER_ELIF_8(FUNC, B, A, C, D, E, F, G, H)\
	EXD_ITER_ELIF_8(FUNC, C, B, A, D, E, F, G, H)\
	EXD_ITER_ELIF_8(FUNC, D, B, C, A, E, F, G, H)\
	EXD_ITER_ELIF_8(FUNC, E, B, C, D, A, F, G, H)\
	EXD_ITER_ELIF_8(FUNC, F, B, C, D, E, A, G, H)\
	EXD_ITER_ELIF_8(FUNC, G, B, C, D, E, F, A, H)\
	EXD_ITER_ELIF_8(FUNC, H, B, C, D, E, F, G, A)

#define EXD_ITER_7(FUNC, A, B, C, D, E, F, G)\
	EXD_USELESS_IF()\
	EXD_ITER_ELIF_7(FUNC, A, B, C, D, E, F, G)\
	EXD_ITER_ELIF_7(FUNC, B, A, C, D, E, F, G)\
	EXD_ITER_ELIF_7(FUNC, C, B, A, D, E, F, G)\
	EXD_ITER_ELIF_7(FUNC, D, B, C, A, E, F, G)\
	EXD_ITER_ELIF_7(FUNC, E, B, C, D, A, F, G)\
	EXD_ITER_ELIF_7(FUNC, F, B, C, D, E, A, G)\
	EXD_ITER_ELIF_7(FUNC, G, B, C, D, E, F, A)\

#define EXD_ITER_6(FUNC, A, B, C, D, E, F)\
	EXD_USELESS_IF()\
	EXD_ITER_ELIF_6(FUNC, A, B, C, D, E, F)\
	EXD_ITER_ELIF_6(FUNC, B, A, C, D, E, F)\
	EXD_ITER_ELIF_6(FUNC, C, B, A, D, E, F)\
	EXD_ITER_ELIF_6(FUNC, D, B, C, A, E, F)\
	EXD_ITER_ELIF_6(FUNC, E, B, C, D, A, F)\
	EXD_ITER_ELIF_6(FUNC, F, B, C, D, E, A)\

#define EXD_ITER_5(FUNC, A, B, C, D, E)\
	EXD_USELESS_IF()\
	EXD_ITER_ELIF_5(FUNC, A, B, C, D, E)\
	EXD_ITER_ELIF_5(FUNC, B, A, C, D, E)\
	EXD_ITER_ELIF_5(FUNC, C, B, A, D, E)\
	EXD_ITER_ELIF_5(FUNC, D, B, C, A, E)\
	EXD_ITER_ELIF_5(FUNC, E, B, C, D, A)\

#define EXD_ITER_4(FUNC, A, B, C, D)\
	EXD_USELESS_IF()\
	EXD_ITER_ELIF_4(FUNC, A, B, C, D)\
	EXD_ITER_ELIF_4(FUNC, B, A, C, D)\
	EXD_ITER_ELIF_4(FUNC, C, B, A, D)\
	EXD_ITER_ELIF_4(FUNC, D, B, C, A)\

#define EXD_ITER_3(FUNC, A, B, C)\
	EXD_USELESS_IF()\
	EXD_ITER_ELIF_3(FUNC, A, B, C)\
	EXD_ITER_ELIF_3(FUNC, B, A, C)\
	EXD_ITER_ELIF_3(FUNC, C, B, A)\

#define EXD_ITER_2(FUNC, A, B)\
	EXD_USELESS_IF()\
	EXD_ITER_ELIF_2(FUNC, A, B)\
	EXD_ITER_ELIF_2(FUNC, B, A)\

#define EXD_ITER_1(FUNC, A)\
for (exd::Entity & ent : (A)->dense_ents)\
{\
	FUNC(ent);\
}


#define EXD_EXPAND(X) X
#define EXD_USELESS_IF() if(0) {}

#define EXD_INTERNAL_GET_ITERATE(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define EXD_ITERATE(FUNC, ...)\
EXD_EXPAND(EXD_INTERNAL_GET_ITERATE(__VA_ARGS__, EXD_ITER_8, EXD_ITER_7, EXD_ITER_6, EXD_ITER_5, EXD_ITER_4, EXD_ITER_3, EXD_ITER_2, EXD_ITER_1)(FUNC, __VA_ARGS__))

#define EXD_EXCL_4(A, B, C, D)\
	if ((A)->has(ent) || (B)->has(ent) || (C)->has(ent) || (D)->has(ent)) return

#define EXD_EXCL_3(A, B, C)\
	if ((A)->has(ent) || (B)->has(ent) || (C)->has(ent)) return

#define EXD_EXCL_2(A, B)\
	if ((A)->has(ent) || (B)->has(ent)) return

#define EXD_EXCL_1(A)\
	if ((A)->has(ent)) return

#define EXD_INTERNAL_GET_EXCLUDE(_1, _2, _3, _4, NAME, ...) NAME
#define EXD_EXCLUDE(...)\
EXD_EXPAND(EXD_INTERNAL_GET_EXCLUDE(__VA_ARGS__, EXD_EXCL_4, EXD_EXCL_3, EXD_EXCL_2, EXD_EXCL_1))(__VA_ARGS__)


}
}