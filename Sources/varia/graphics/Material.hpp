#pragma once

#include "kinc/graphics4/pipeline.h"
#include "kinc/graphics4/constantlocation.h"
#include "kinc/graphics4/textureunit.h"
#include "kinc/graphics4/graphics.h"

#include "Program.hpp"


struct varia_graphics_textured_material_t
{
	varia_graphics_program_textured_t * textured_program;
	varia_graphics_uniform_tex_t sampler;
	varia_graphics_uniform_mvp_t mvp;
};

varia_graphics_textured_material_t varia_graphics_material_create_textured(kinc_g4_texture_t tex, kinc_matrix4x4_t mvp);
void varia_graphics_material_update_texture(varia_graphics_textured_material_t * material, kinc_g4_texture_t tex);
void varia_graphics_material_update_mvp(varia_graphics_textured_material_t * material, kinc_matrix4x4_t mvp);
