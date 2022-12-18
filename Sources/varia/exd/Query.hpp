#pragma once

#include "varia/vcommon.hpp"
#include "varia/ds/Bitset.hpp"
#include "varia/exd/Entity.hpp"
#include "varia/exd/World.hpp"

namespace exd
{

template<int Size>
struct Query
{

	struct QueryIterator
	{
		private:
			vds::Bitset32<Size> * drain_iter;
			World<Size> * w;
			Entity current_ent;

		public:
			QueryIterator(Query<Size> * q, Entity ent) 
			{ 
				this->drain_iter = &q->drain_iter;
				this->w = q->w;
				this->current_ent = ent;

				//Note(zshoals Dec-18-2022):> Ignore "INVALID_ENTITY" at slot 0 
			}
			QueryIterator operator++()
			{
				vds::Result<size_t> res = drain_iter->find_first_set();
				this->current_ent = w->internal_lookup_entity_via_id(res.value);

				this->drain_iter->unset(res.value);
				this->current_ent.id *= res.valid; //INVALID_ENTITY if invalid, current_ent if valid

				return *this;
			}
			bool operator!=(QueryIterator const & other) 
			{ 
				Entity copy = other.current_ent;
				return !this->current_ent.matches(copy);
			}
			Entity & operator*() { return current_ent; }
	};

	vds::Bitset32<Size> iteration_list;
	vds::Bitset32<Size> drain_iter;
	World<Size> * w;

	Query(World<Size> * w)
	{
		iteration_list = w->entities.bitset;
		this->w = w;
	}

	QueryIterator begin(void) 
	{ 
		vds::Result<size_t> res = drain_iter.find_first_set();

		Entity ent;
		ent.id = 0;
		if (res.valid) 
		{ 
			ent.id = res.value;
			drain_iter.unset(ent.id);
		}

		return QueryIterator(this, ent); 
	}
	QueryIterator end(void) { return QueryIterator(this, INVALID_ENTITY); }

	template<typename T, int Size>
	void include(Component<T, Size> * comp)
	{
		vds::Bitset32<Size> & comp_ents = *w->internal_bitset_lookup(comp->bitset_handle);
		iteration_list.and(comp_ents);
	}

	template<typename T, int Size>
	void exclude(Component<T, Size> * comp)
	{
		vds::Bitset32<Size> & comp_ents = *w->internal_bitset_lookup(comp->bitset_handle);
		iteration_list.andNot(comp_ents);
	}

	void compile(void)
	{
		drain_iter = iteration_list;
		this->drain_iter.unset(0);
	}

};

}