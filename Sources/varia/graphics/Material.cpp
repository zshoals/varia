#include "varia/graphics/Material.hpp"

#include "varia/graphics/Program.hpp"
#include "varia/util/Memory.hpp"


varia_graphics_material_t varia_graphics_material_create_from(varia_graphics_program_t * program)
{
	varia_graphics_material_t mat;
	mat.compiled = false;
	mat.program = program;

	vds_array_initialize_direct(&mat.uniforms, &mat._uniforms_backing_storage[0], VARIA_GRAPHICS_MATERIAL_MAX_UNIFORMS);
	vds_array_initialize_direct(&mat.samplers, &mat._samplers_backing_storage[0], VARIA_GRAPHICS_MATERIAL_MAX_SAMPLERS);

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

void varia_graphics_material_add_uniform_vec2(varia_graphics_material_t * material, char const * identifier, vec2s v)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::VEC2;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);

	kinc_vector2_t kv2;
	varia_bitcast(&kv2, &v, sizeof(kv2));
	uniform.vec2 = kv2;

	vds_array_push(&material->uniforms, uniform);
}

void varia_graphics_material_add_uniform_vec3(varia_graphics_material_t * material, char const * identifier, vec3s v)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::VEC3;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);
	kinc_vector3_t kv3;
	varia_bitcast(&kv3, &v, sizeof(kv3));
	uniform.vec3 = kv3;

	vds_array_push(&material->uniforms, uniform);
}

void varia_graphics_material_add_uniform_mat3(varia_graphics_material_t * material, char const * identifier, mat3s mat)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::MAT3;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);

	kinc_matrix3x3_t kmat3;
	varia_bitcast(&kmat3, &mat, sizeof(kmat3));
	uniform.mat3 = kmat3;

	vds_array_push(&material->uniforms, uniform);
}

void varia_graphics_material_add_uniform_mat4(varia_graphics_material_t * material, char const * identifier, mat4s mat)
{
	ENSURE(!material->compiled, "varia_graphics_material:> Tried to add a sampler or uniform after compiling a material. Only sampler and uniform updates are permitted.");

	varia_graphics_uniform_t uniform;

	uniform.tag = varia_graphics_uniform_type_e::MAT4;
	uniform.identifier = identifier;
	uniform.location = kinc_g4_pipeline_get_constant_location(&material->program->pipe, identifier);

	kinc_matrix4x4_t kmat4;
	varia_bitcast(&kmat4, &mat, sizeof(kmat4));
	uniform.mat4 = kmat4;

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

void varia_graphics_material_update_uniform_vec2(varia_graphics_material_t * material, char const * identifier, vec2s v)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");
	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		bool typematch = elem->tag == varia_graphics_uniform_type_e::VEC2;
		return (compare == 0) && typematch;
	});

	if (search)
	{
		kinc_vector2_t kv2;
		varia_bitcast(&kv2, &v, sizeof(kv2));
		(*search)->vec2 = kv2;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a uniform that didn't exist or had a type mismatch for this identifier.");
	}
}

void varia_graphics_material_update_uniform_vec3(varia_graphics_material_t * material, char const * identifier, vec3s v)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");

	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		bool typematch = elem->tag == varia_graphics_uniform_type_e::VEC3;
		return (compare == 0) && typematch;
	});

	if (search)
	{
		kinc_vector3_t kv3;
		varia_bitcast(&kv3, &v, sizeof(kv3));
		(*search)->vec3 = kv3;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a uniform that didn't exist or had a type mismatch for this identifier.");
	}
}

void varia_graphics_material_update_uniform_mat3(varia_graphics_material_t * material, char const * identifier, mat3s mat)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");

	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		bool typematch = elem->tag == varia_graphics_uniform_type_e::MAT3;
		return (compare == 0) && typematch;
	});

	if (search)
	{
		kinc_matrix3x3_t kmat3;
		varia_bitcast(&kmat3, &mat, sizeof(kmat3));
		(*search)->mat3 = kmat3;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_material:> Tried to update a uniform that didn't exist or had a type mismatch for this identifier.");
	}
}

void varia_graphics_material_update_uniform_mat4(varia_graphics_material_t * material, char const * identifier, mat4s mat)
{
	ENSURE(material->compiled, "varia_graphics_material:> Tried to update a sampler or uniform before compiling a material.");

	vds_option_t<varia_graphics_uniform_t *> search = vds_array_find_get(&material->uniforms, [&identifier](varia_graphics_uniform_t * elem)
	{
		int compare = strcmp(elem->identifier, identifier);
		bool typematch = elem->tag == varia_graphics_uniform_type_e::MAT4;
		return (compare == 0) && typematch;
	});

	if (search)
	{
		kinc_matrix4x4_t kmat4;
		varia_bitcast(&kmat4, &mat, sizeof(kmat4));
		(*search)->mat4 = kmat4;
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



varia_graphics_material_t varia_graphics_material_create_default_textured(kinc_g4_texture_t tex, mat4s mvp)
{
	varia_graphics_material_t material = varia_graphics_material_create_from(varia_graphics_program_get_textured_program());
	varia_graphics_material_add_sampler(&material, "tex", tex);
	varia_graphics_material_add_uniform_float(&material, "time", 0.0f);
	varia_graphics_material_add_uniform_mat4(&material, "projectionMatrix", mvp);
	varia_graphics_material_compile(&material);

	return material;
}
