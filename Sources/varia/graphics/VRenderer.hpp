#pragma once

#include "varia/VShared.hpp"

struct Graphics_Intermediate_Representation
{
    int renderables[128];
};

struct Graphics_Renderer
{
    Boolean no;
};

void v_graphics_renderer_render(Graphics_Renderer * gfx, Graphics_Intermediate_Representation const * ir);