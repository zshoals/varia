#include "varia/graphics/Material.hpp"

#include "varia/graphics/Program.hpp"



varia_graphics_textured_material_t varia_graphics_material_create_textured(kinc_g4_texture_t tex, kinc_matrix4x4_t mvp)
{
	varia_graphics_textured_material_t material;
	{
		material.textured_program = varia_graphics_program_get_textured_program();

		varia_graphics_uniform_tex_t uniform_tex;
		uniform_tex.tex = tex;
		uniform_tex.sampler = kinc_g4_pipeline_get_texture_unit(&material.textured_program->pipe, "tex");

		varia_graphics_uniform_mvp_t uniform_mvp;
		uniform_mvp.mat4 = mvp;
		uniform_mvp.mvp = kinc_g4_pipeline_get_constant_location(&material.textured_program->pipe, "projectionMatrix");


		material.sampler = uniform_tex;
		material.mvp = uniform_mvp;
	}

	return material;
}

void varia_graphics_material_update_texture(varia_graphics_textured_material_t * material, kinc_g4_texture_t tex)
{
	material->sampler.tex = tex;
}

void varia_graphics_material_update_mvp(varia_graphics_textured_material_t * material, kinc_matrix4x4_t mvp)
{
	material->mvp.mat4 = mvp;
}

