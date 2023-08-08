#pragma once

#include "kinc/graphics4/vertexstructure.h"
#include "kinc/graphics4/pipeline.h"

struct Textured_Pipeline
{
    kinc_g4_pipeline_t pipe;
    kinc_g4_vertex_structure_t structure;
    //NOTE(<zshoals> 08-05-2023): We could actually store the names of the constants and
    //  texture units directly onto this structure as short strings or something
    kinc_g4_texture_unit_t tex;

    //NOTE(<zshoals> 08-05-2023): Use standard types here as these will be passed into the GPU as uniforms
    //  may as well be pessimistic about the situation
    //TODO(<zshoals> 08-07-2023): We can add a callback here that actually applies all the uniforms
    //to G4, which we trigger whenever we switch to a new pipeline
    float time;
    kinc_g4_constant_location_t time_location;
};

void v_pipeline_initialize_textured(Textured_Pipeline * pipe, kinc_g4_shader_t * vertex_shader, kinc_g4_shader_t * fragment_shader);