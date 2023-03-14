#include "varia/Vcommon.hpp"
#include "kinc/graphics4/pipeline.h"
#include "kinc/graphics4/vertexstructure.h"
#include "kinc/math/matrix.h"
#include "kinc/graphics4/texture.h"
#include "kinc/graphics4/shader.h"

#pragma once

struct varia_graphics_uniform_mvp_t
{
	kinc_matrix4x4_t mat4;
	kinc_g4_constant_location_t mvp;
};

struct varia_graphics_uniform_tex_t
{
	kinc_g4_texture_t tex;
	kinc_g4_texture_unit_t sampler;
};

struct varia_graphics_program_textured_t
{
	kinc_g4_pipeline_t pipe;

	kinc_g4_vertex_structure_t position;
	kinc_g4_vertex_structure_t color;
	kinc_g4_vertex_structure_t uv;

	kinc_g4_shader_t vertex_shader;
	kinc_g4_shader_t fragment_shader;
};

varia_graphics_program_textured_t * varia_graphics_program_get_textured_program(void);
void varia_graphics_program_initialize_defaults(void);