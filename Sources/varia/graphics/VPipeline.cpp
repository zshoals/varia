#include "varia/graphics/VPipeline.hpp"

#include "varia/VShared.hpp"

#include "kinc/graphics4/vertexstructure.h"
#include "kinc/graphics4/pipeline.h"

void v_pipeline_initialize_textured(Textured_Pipeline * pipe, kinc_shader_g4_t * vertex_shader, kinc_shader_g4_t * fragment_shader)
{
    kinc_g4_pipeline_init(address_of(pipe->pipe);

    kinc_g4_vertex_structure_t * vs = address_of(pipe->structure);
    kinc_g4_vertex_structure_init(vs);
    kinc_g4_vertex_structure_add(vs, "pos", KINC_G4_VERTEX_DATA_F32_2X);
    kinc_g4_vertex_structure_add(vs, "uv", KINC_G4_VERTEX_DATA_F32_2X);
    kinc_g4_vertex_structure_add(vs, "layer", KINC_G4_VERTEX_DATA_F32_1X);
    kinc_g4_vertex_structure_add(vs, "color", KINC_G4_VERTEX_DATA_I32_1X);

    pipe->pipe.input_layout[0] = vs;
    pipe->pipe.vertex_shader = vertex_shader;
    pipe->pipe.fragment_shader = fragment_shader;

    pipe->pipe.blend_source = KINC_G4_BLEND_SOURCE_COLOR;
    pipe->pipe.blend_destination = KINC_G4_BLEND_INV_SOURCE_COLOR;

    kinc_g4_pipeline_compile(address_of(pipe->pipe));

    pipe->tex = kinc_g4_pipeline_get_texture_unit(address_of(pipe->pipe), "tex");
    pipe->time = kinc_g4_pipeline_get_constant_location(address_of(pipe->pipe), "time");
    //TODO(<zshoals> 08-04-2023): Add some constant locations!
}