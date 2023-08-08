#include "varia/graphics/VRenderer.hpp"

#include "kinc/graphics4/graphics.h"
#include "varia/graphics/VPipeline.hpp"
#include "varia/VAssets.hpp"

void v_graphics_initialize(Graphics_State * graphics, Assets * assets)
{
    //Initialize Vertex structure
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    kinc_g4_vertex_structure_init(address_of(graphics->vertex_layout));
    // kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "pos", KINC_G4_VERTEX_DATA_F32_2X);
    // kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "layer", KINC_G4_VERTEX_DATA_F32_1X);
    // kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "color", KINC_G4_VERTEX_DATA_U32_1X);
    // kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "uv", KINC_G4_VERTEX_DATA_F32_2X);

    kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "vertexPosition", KINC_G4_VERTEX_DATA_F32_3X);
    kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "vertexUV", KINC_G4_VERTEX_DATA_F32_2X);
    kinc_g4_vertex_structure_add(address_of(graphics->vertex_layout), "vertexColor", KINC_G4_VERTEX_DATA_F32_4X);
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Set the atlas and configure the default pipeline
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    graphics->active_texture = address_of(assets->atlas.texture);

    v_pipeline_initialize_textured
    (
        address_of(graphics->tex_pipe),
        address_of(graphics->vertex_layout),
        address_of(assets->textured_vert),
        address_of(assets->textured_frag)
    );

    graphics->tex_pipe.update_callback(address_of(graphics->tex_pipe), address_of(assets->atlas.texture));
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Initialize Vertex and Index Buffers
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    int vertex_count = 10000 * 4;
    int index_count = 10000 * 6;

    kinc_g4_vertex_buffer_init
    (
        address_of(graphics->vbo),
        vertex_count,
        address_of(graphics->vertex_layout),
        KINC_G4_USAGE_DYNAMIC,
        0
    );

    kinc_g4_index_buffer_init
    (
        address_of(graphics->ibo),
        index_count,
        KINC_G4_INDEX_BUFFER_FORMAT_16BIT,
        KINC_G4_USAGE_STATIC
    );

    Signless_16 * ibo_data = static_cast<Signless_16 *>( kinc_g4_index_buffer_lock_all(address_of(graphics->ibo)) );
    for (Integer_64 i = 0; i < index_count; i += 6)
    {
        ibo_data[i + 0] = 0;
        ibo_data[i + 1] = 1;
        ibo_data[i + 2] = 2;
        ibo_data[i + 3] = 2;
        ibo_data[i + 4] = 1;
        ibo_data[i + 5] = 3;
    }
    kinc_g4_index_buffer_unlock_all(address_of(graphics->ibo));
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}

void v_graphics_renderer_render(Graphics_State * graphics, Graphics_Intermediate_Representation const * ir)
{
    graphics->tex_pipe.update_callback(address_of(graphics->tex_pipe), graphics->active_texture);

    //TODO(<zshoals> 08-03-2023): Do rendering
    kinc_g4_begin(0);
    kinc_g4_end(0);
    kinc_g4_swap_buffers();
}