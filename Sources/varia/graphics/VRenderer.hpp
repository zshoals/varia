#pragma once

#include "varia/VShared.hpp"

#include "varia/graphics/VAtlas.hpp"
#include "varia/graphics/VGraphicsIR.hpp"
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


struct Graphics_State
{
    Atlas active_atlas;

    kinc_g4_vertex_structure_t vertex_layout;
    kinc_g4_vertex_buffer_t vbo;
    kinc_g4_index_buffer_t ibo;
    Textured_Pipeline tex_pipe;

    kinc_g4_texture_unit_t active_texture_unit;
};

void v_graphics_initialize(Graphics_State * graphics, Assets * assets);

void v_graphics_renderer_render(Graphics_State * graphics, Gamestate * visual_world, Graphics_Intermediate_Representation const * ir);
void v_graphics_ir_build();
void v_graphics_ir_clear();

