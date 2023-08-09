#include "varia/graphics/VRenderer.hpp"

#include "varia/graphics/VGraphicsIR.hpp"
#include "kinc/graphics4/graphics.h"
#include "varia/graphics/VPipeline.hpp"
#include "varia/VAssets.hpp"

static inline void v_graphics_push_vert(float * vbo_base, float x, float y, float z, float u, float v, float r, float g, float b, float a)
{
    //Position
    vbo_base[0] = x;
    vbo_base[1] = y;
    vbo_base[2] = z;
    //UV
    vbo_base[3] = u;
    vbo_base[4] = v;
    //Color
    vbo_base[5] = r;
    vbo_base[6] = g;
    vbo_base[7] = b;
    vbo_base[8] = a;
}

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
    v_atlas_initialize
    (
        address_of(graphics->active_atlas), 
        address_of(assets->atlas_texture_01),
        const_address_of(assets->atlas_metadata_01)
    );

    v_pipeline_initialize_textured
    (
        address_of(graphics->tex_pipe),
        address_of(graphics->vertex_layout),
        address_of(assets->textured_vert.shader),
        address_of(assets->textured_frag.shader)
    );
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Initialize Vertex and Index Buffers
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    int quad_count = 2000;
    int vertex_count = quad_count * 4;
    int index_count = quad_count * 6;

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
    kinc_g4_vertex_buffer_t * vbo = address_of(graphics->vbo);

    float * vbo_data = kinc_g4_vertex_buffer_lock_all(vbo);
    {
        /*
        
            0-----------2
            |           |
            |           |
            |           |
            1-----------3
        
        */
        float u_min = 0.14746f;
        float v_min = 0.170166f;
        float u_max = 0.24536f;
        float v_max = 0.183349f;

        v_graphics_push_vert(vbo_data,    -1.0, -1.0, 0.5,      u_min, v_min,       1.0, 1.0, 1.0, 1.0);
        vbo_data += 9;
        v_graphics_push_vert(vbo_data,    -1.0, 1.0, 0.5,      u_min, v_max,       1.0, 1.0, 1.0, 1.0);
        vbo_data += 9;
        v_graphics_push_vert(vbo_data,    1.0, -1.0, 0.5,      u_max, v_min,       1.0, 1.0, 1.0, 1.0);
        vbo_data += 9;
        v_graphics_push_vert(vbo_data,    1.0, 1.0, 0.5,      u_max, v_max,       1.0, 1.0, 1.0, 1.0);
    }
    kinc_g4_vertex_buffer_unlock_all(vbo);

    //Actually Render
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    kinc_g4_begin(0);
    {
        graphics->tex_pipe.update_callback(address_of(graphics->tex_pipe), v_atlas_get_texture(address_of(graphics->active_atlas)));
        kinc_g4_set_vertex_buffer(vbo);
        kinc_g4_set_index_buffer(address_of(graphics->ibo));

        //TODO(<zshoals> 08-08-2023): Hardcoded experiment
        kinc_g4_draw_indexed_vertices_from_to(0, 6);
    }
    kinc_g4_end(0);
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    kinc_g4_swap_buffers();
}