#pragma once

#include "varia/exd/Component.hpp"
#include "varia/exd/Helpers.hpp"
#include "varia/comps/Position.hpp"

struct PositionUpdate
{
	exd::Component<Position> * pos1;
	exd::Component<Position> * pos2;

	PositionUpdate(exd::Component<Position> * posa, exd::Component<Position> * posb)
	{
		pos1 = posa;
		pos2 = posb;
	}

	void processor(exd::Entity ent)
	{
		Position * p = pos1->get_mut(ent);
		p->x += 100;
		p->y += 747547;
		VARIA_QLOG("Hello! We hit here once!");
	}

	void run(void)
	{
		EXD_ITERATE(processor, pos1, pos2);
	}
};