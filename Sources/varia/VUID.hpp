#pragma once

#include "varia/VShared.hpp"

typedef Integer_64 UID;

struct UID_State
{
    UID UID_Generator;
    Boolean maxed;
};

static inline UID v_UID_next(UID_State * state)
{
    if (UID_Generator >= 0xFFFFFFFFFFFFFFF0)
    {
        state->maxed = true;
    }

    if (!(state->maxed))
    {
        state->UID_Generator += 1;
        return state->UID_Generator;
    }
    else
    {
        return 0;
    }
}