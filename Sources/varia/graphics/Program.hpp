#include "varia/Vcommon.hpp"
#include "kinc/graphics4/pipeline.h"
#include "kinc/graphics4/vertexstructure.h"
#include "kinc/math/matrix.h"
#include "kinc/graphics4/texture.h"
#include "kinc/graphics4/shader.h"

#include "varia/ds/StaticArray.hpp"

#pragma once

#define VARIA_GRAPHICS_PROGRAM_MAX_VERTEX_STRUCTURES 16

struct varia_graphics_program_t
{
	kinc_g4_pipeline_t pipe;

	kinc_g4_shader_t vertex_shader;
	kinc_g4_shader_t fragment_shader;

	vds_array_t<kinc_g4_vertex_structure_t> vertex_structures; //16 of them
};

varia_graphics_program_t * varia_graphics_program_get_textured_program(void);
void varia_graphics_program_initialize_defaults(void);