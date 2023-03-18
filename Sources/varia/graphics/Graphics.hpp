#pragma once

#include "varia/graphics/Program.hpp"
#include "varia/graphics/Material.hpp"
#include "varia/graphics/Camera.hpp"

#include "kinc/graphics4/indexbuffer.h"
#include "kinc/graphics4/vertexbuffer.h"
#include "kinc/graphics4/graphics.h"

struct varia_graphics_textured_painter_t
{
	kinc_g4_vertex_buffer_t texPos;
	kinc_g4_vertex_buffer_t texColor;
	kinc_g4_vertex_buffer_t texUV;

	kinc_g4_index_buffer_t texIndex;
};

struct varia_graphics_t
{
	varia_graphics_textured_painter_t texPainter;
};

void varia_graphics_camera_set(mat4s cam);
void varia_graphics_batch_begin(varia_graphics_material_t mat);
void varia_graphics_batch_end(void);