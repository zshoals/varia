#pragma once

#include "varia/VShared.hpp"

#include "varia/graphics/VAtlas.hpp"
#include "varia/graphics/VPipeline.hpp"
#include "varia/VAssets.hpp"
#include "varia/ds/VDS-Array.hpp"
#include "kinc/graphics4/texture.h"
#include "kinc/graphics4/vertexstructure.h"
#include "kinc/graphics4/vertexbuffer.h"
#include "kinc/graphics4/indexbuffer.h"
#include "kinc/graphics4/textureunit.h"
#include "kinc/graphics4/constantlocation.h"
#include "kinc/graphics4/graphics.h"
#include "varia/VGamestate.hpp"
#include "varia/graphics/VGraphicsItem.hpp"

struct Graphics_IR
{
    Integer_64 count;

    VDS_Array<Graphics_Renderable,          1024> items;
    VDS_Array<Graphics_Renderable_Sortable, 1024> sortables;
};

struct Graphics_State
{
    Atlas active_atlas;

    kinc_g4_vertex_structure_t vertex_layout;
    kinc_g4_vertex_buffer_t vbo;
    kinc_g4_index_buffer_t ibo;
    Textured_Pipeline tex_pipe;

    kinc_g4_texture_unit_t active_texture_unit;

    Graphics_IR ir;
};

void v_graphics_initialize(Graphics_State * graphics, Assets * assets);
void v_graphics_render(Graphics_State * graphics, Gamestate * visual_world);