
#pragma once

#include "varia/exd/Component.hpp"
#include "varia/exd/Helpers.hpp"
#include "varia/comps/Position.hpp"

struct PositionReader
{
	exd::Component<Position> * pos1;
	exd::Component<Position> * pos2;

	PositionReader(exd::Component<Position> * posa, exd::Component<Position> * posb)
	{
		pos1 = posa;
		pos2 = posb;
	}

	inline void processor(exd::Entity ent)
	{
		EXD_EXCLUDE(pos1);
		Position const * p = pos1->get(ent);
		VARIA_LOG_INT(p->x);
		VARIA_LOG_INT(p->x);
		VARIA_LOG_INT(p->x);
		VARIA_LOG_INT(p->x);
		VARIA_LOG_INT(p->x);
	}

	void run(void)
	{
		EXD_ITERATE(processor, pos1, pos2);
	}
};