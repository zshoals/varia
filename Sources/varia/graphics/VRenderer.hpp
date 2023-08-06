#pragma once

#include "varia/VShared.hpp"

#include "kinc/graphics4/texture.h"
#include "varia/graphics/VPipeline.hpp"
#include "kinc/graphics4/vertexstructure.h"
#include "kinc/graphics4/vertexbuffer.h"
#include "kinc/graphics4/indexbuffer.h"
#include "kinc/graphics4/graphics.h"

struct Graphics_Intermediate_Representation
{
    int renderables[128];
};

struct Graphics_Renderer
{
    Textured_Pipeline tex_pipe;
    kinc_g4_texture_t atlas;
    //NOTE(<zshoals> 08-03-2023): Layout -- X, Y, DEPTH, COLOR, ATLAS_U, ATLAS_V, (maybe ATLAS_INDEX if texture array later?)
    kinc_g4_vertex_structure_t vertex_layout;
    kinc_g4_vertex_buffer_t vbo;
    kinc_g4_index_buffer_t ibo;
};

void v_graphics_initialize(Graphics_Renderer * graphics, Integer_32 max_quads);

void v_graphics_renderer_render(Graphics_Renderer * gfx, Graphics_Intermediate_Representation const * ir);