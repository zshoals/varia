#include "varia/graphics/VRenderer.hpp"

#include "kinc/graphics4/graphics.h"

void v_graphics_initialize(Graphics_Renderer * graphics)
{
    kinc_g4_vertex_structure_init(address_of(graphics->vertex_layout));
    kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "pos", KINC_G4_VERTEX_DATA_F32_2X);
    kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "layer", KINC_G4_VERTEX_DATA_F32_1X);
    kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "color", KINC_G4_VERTEX_DATA_U32_1X);
    kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "uv", KINC_G4_VERTEX_DATA_F32_2X);


}

void v_graphics_renderer_render(Graphics_Renderer * gfx, Graphics_Intermediate_Representation const * ir)
{
    //TODO(<zshoals> 08-03-2023): Do rendering
    kinc_g4_begin(0);
    kinc_g4_end(0);
    kinc_g4_swap_buffers();
}