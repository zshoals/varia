#pragma once

#include "kinc/graphics4/vertexstructure.h"
#include "kinc/graphics4/pipeline.h"

struct Textured_Pipeline
{
    kinc_g4_pipeline_t pipe;
    kinc_g4_vertex_structure_t structure;
    kinc_g4_texture_unit_t tex;
    kinc_g4_constant_location_t time;
};

void v_pipeline_initialize_textured(Textured_Pipeline * pipe, kinc_shader_g4_t * vertex_shader, kinc_shader_g4_t * fragment_shader);