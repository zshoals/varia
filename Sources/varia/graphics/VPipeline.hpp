#pragma once

#include "kinc/graphics4/vertexstructure.h"
#include "kinc/graphics4/pipeline.h"
#include "kinc/graphics4/texture.h"
#include "varia/VAssets.hpp"

struct Textured_Pipeline;

//TODO(<zshoals> 08-07-2023): Should support any pipeline?
using Pipeline_Update_Callback = void(*)(Textured_Pipeline * pipe, kinc_g4_texture_t * texture, float time);

struct Textured_Pipeline
{
    kinc_g4_pipeline_t pipe;
    kinc_g4_vertex_structure_t structure;

    //NOTE(<zshoals> 08-05-2023): Use standard types here as these will be passed into the GPU as uniforms
    //  may as well be pessimistic about the situation
    //"time"
    kinc_g4_constant_location_t time_location;
    float time;

    kinc_g4_texture_unit_t texture_unit;

    Pipeline_Update_Callback update_callback;
};

void v_pipeline_initialize_textured(Textured_Pipeline * pipe, kinc_g4_vertex_structure_t * vs, kinc_g4_shader_t * vertex_shader, kinc_g4_shader_t * fragment_shader);
