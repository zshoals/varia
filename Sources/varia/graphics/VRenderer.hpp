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

struct Graphics_Item
{
    float x;
    float y;
    //NOTE(<zshoals> 08-09-2023): Layer is duplicated to avoid having to somehow get 
    //  this value from Graphics_Item_Sortable later
    float layer;
    float w;
    float h;
    //Note: before submitting the graphics item, we must resolve the uv coordinates
    float u_min;
    float v_min;
    float u_max;
    float v_max;
    float r;
    float g;
    float b;
    float a;
};

struct Graphics_Item_Sortable
{
    float layer;
    Graphics_Item const * item;
};

struct Graphics_IR
{
    Integer_64 count;

    VDS_Array<Graphics_Item,          1024> items;
    VDS_Array<Graphics_Item_Sortable, 1024> sortables;
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