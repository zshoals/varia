#include "varia/graphics/Material.hpp"

#include "varia/graphics/Program.hpp"


varia_graphics_material_t varia_graphics_material_create_from(varia_graphics_program_t * program)
{
	varia_graphics_material_t mat;
	mat.compiled = false;
	mat.program = program;

	vds_array_initialize_direct(&mat.uniforms, &mat.uniforms_backing_storage[0], VARIA_GRAPHICS_MATERIAL_MAX_UNIFORMS);
	vds_array_initialize_direct(&mat.samplers, &mat.samplers_backing_storage[0], VARIA_GRAPHICS_MATERIAL_MAX_SAMPLERS);

	return mat;
}

void varia_graphics_material_compile(varia_graphics_material_t * material)
{
	material->compiled = true;
}

void varia_graphics_material_add_sampler(varia_graphics_material_t * material, char const * identifier, kinc_g4_texture_t tex)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_sampler_t sampler;

	sampler.identifier = identifier;
	//TODO(zshoals 03-14-2023):> Figure out a way to check if the texture unit was found?
	//Kinc doesn't seem to support this right now, maybe check for -1 in its implementation?
	sampler.location = kinc_g4_pipeline_get_texture_unit(&material->program->pipe, identifier);
	sampler.texture = tex;

	vds_array_push(&material->samplers, sampler);
}

void varia_graphics_material_add_uniform_float(varia_graphics_material_t * material, char const * identifier, float f)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::FLOAT;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);
	uniform.f = f;

	vds_array_push(&material->uniforms, uniform);
}

void varia_graphics_material_add_uniform_vec2(varia_graphics_material_t * material, char const * identifier, kinc_vector2_t v)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::VEC2;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);
	uniform.vec2 = v;

	vds_array_push(&material->uniforms, uniform);
}

void varia_graphics_material_add_uniform_vec3(varia_graphics_material_t * material, char const * identifier, kinc_vector3_t v)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::VEC3;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);
	uniform.vec3 = v;

	vds_array_push(&material->uniforms, uniform);
}

void varia_graphics_material_add_uniform_mat3(varia_graphics_material_t * material, char const * identifier, kinc_matrix3x3_t mat)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::MAT3;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);
	uniform.mat3 = mat;

	vds_array_push(&material->uniforms, uniform);
}

void varia_graphics_material_add_uniform_mat4(varia_graphics_material_t * material, char const * identifier, kinc_matrix4x4_t mat)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::MAT4;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);
	uniform.mat4 = mat;

	vds_array_push(&material->uniforms, uniform);
}




//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||        Uniform Updates                                              ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

void varia_graphics_material_update_sampler(varia_graphics_material_t * material, char const * identifier, kinc_g4_texture_t tex)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");

	vds_option_t<varia_graphics_sampler_t *> search = vds_array_find_get(&material->samplers, [&identifier](varia_graphics_sampler_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		return (compare == 0);
	});

	if (search)
	{
		(*search)->texture = tex;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a sampler that didn't exist in this shader program.");
	}
}

void varia_graphics_material_update_uniform_float(varia_graphics_material_t * material, char const * identifier, float f)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");
	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		return (compare == 0);
	});

	if (search && ( (*search)->tag == varia_graphics_uniform_type_e::FLOAT ))
	{
		(*search)->f = f;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a uniform that didn't exist or had a type mismatch for this identifier.");
	}
}

void varia_graphics_material_update_uniform_vec2(varia_graphics_material_t * material, char const * identifier, kinc_vector2_t v)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");
	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		return (compare == 0);
	});

	if (search && ( (*search)->tag == varia_graphics_uniform_type_e::VEC2 ))
	{
		(*search)->vec2 = v;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a uniform that didn't exist or had a type mismatch for this identifier.");
	}
}

void varia_graphics_material_update_uniform_vec3(varia_graphics_material_t * material, char const * identifier, kinc_vector3_t v)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");

	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		return (compare == 0);
	});

	if (search && ( (*search)->tag == varia_graphics_uniform_type_e::VEC3 ))
	{
		(*search)->vec3 = v;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a uniform that didn't exist or had a type mismatch for this identifier.");
	}
}

void varia_graphics_material_update_uniform_mat3(varia_graphics_material_t * material, char const * identifier, kinc_matrix3x3_t mat)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");

	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		return (compare == 0);
	});

	if (search && ( (*search)->tag == varia_graphics_uniform_type_e::MAT3 ))
	{
		(*search)->mat3 = mat;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a uniform that didn't exist or had a type mismatch for this identifier.");
	}
}

void varia_graphics_material_update_uniform_mat4(varia_graphics_material_t * material, char const * identifier, kinc_matrix4x4_t mat)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");

	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		return (compare == 0);
	});

	if (search && ( (*search)->tag == varia_graphics_uniform_type_e::MAT4 ))
	{
		(*search)->mat4 = mat;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a uniform that didn't exist or had a type mismatch for this identifier.");
	}
}




//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||            Default Materials                                        ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||



varia_graphics_material_t varia_graphics_material_create_default_textured(kinc_g4_texture_t tex, kinc_matrix4x4_t mvp)
{
	varia_graphics_material_t material = varia_graphics_material_create_from(varia_graphics_program_get_textured_program());
	varia_graphics_material_add_sampler(&material, "tex", tex);
	varia_graphics_material_add_uniform_mat4(&material, "projectionMatrix", mvp);
	varia_graphics_material_compile(&material);

	return material;
}