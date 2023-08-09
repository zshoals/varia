#pragma once

#include "varia/ds/VDS-Array.hpp"
#include "varia/VGamestate.hpp"
#include "varia/graphics/VAtlas.hpp"

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

struct Graphics_Intermediate_Representation
{
    Integer_64 count;

    VDS_Array<Graphics_Item,          1024> items;
    VDS_Array<Graphics_Item_Sortable, 1024> sortables;
};

void v_graphics_ir_clear(Graphics_Intermediate_Representation * ir);
void v_graphics_ir_build(Graphics_Intermediate_Representation * ir, Gamestate const * visual_world, Atlas const * atlas);