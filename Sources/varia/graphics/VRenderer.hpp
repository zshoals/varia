#pragma once

#include "varia/VShared.hpp"

#include "varia/VAssets.hpp"
#include "varia/graphics/VPipeline.hpp"
#include "kinc/graphics4/texture.h"
#include "kinc/graphics4/vertexstructure.h"
#include "kinc/graphics4/vertexbuffer.h"
#include "kinc/graphics4/indexbuffer.h"
#include "kinc/graphics4/textureunit.h"
#include "kinc/graphics4/constantlocation.h"
#include "kinc/graphics4/graphics.h"

struct Graphics_Intermediate_Representation
{
    int renderables[128];
};

struct Graphics_State
{
    Textured_Pipeline tex_pipe;
    kinc_g4_texture_unit_t active_texture_unit;
    kinc_g4_texture_t * active_texture;

    kinc_g4_texture_t atlas;

    //NOTE(<zshoals> 08-03-2023): Layout -- X, Y, DEPTH, COLOR, ATLAS_U, ATLAS_V, (maybe ATLAS_INDEX if texture array later?)
    kinc_g4_vertex_structure_t vertex_layout;
    kinc_g4_vertex_buffer_t vbo;
    kinc_g4_index_buffer_t ibo;

};

void v_graphics_initialize(Graphics_State * graphics, Assets * assets);

void v_graphics_renderer_render(Graphics_State * graphics, Graphics_Intermediate_Representation const * ir);