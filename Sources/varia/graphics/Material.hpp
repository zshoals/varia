#pragma once

#include "kinc/graphics4/pipeline.h"
#include "kinc/graphics4/constantlocation.h"
#include "kinc/graphics4/textureunit.h"
#include "kinc/graphics4/graphics.h"
#include "varia/ds/Option.hpp"
#include "varia/ds/StaticArray.hpp"

#include "varia/lib/cglm/types-struct.h"

#include "Program.hpp"

#define VARIA_GRAPHICS_MATERIAL_MAX_UNIFORMS 16
#define VARIA_GRAPHICS_MATERIAL_MAX_SAMPLERS 8

enum class varia_graphics_uniform_type_e
{
	FLOAT,
	VEC2,
	VEC3,
	MAT3,
	MAT4,
};

struct varia_graphics_uniform_t
{
	char const * identifier;
	kinc_g4_constant_location_t location;

	varia_graphics_uniform_type_e tag;
	union
	{
		float f;
		kinc_vector2_t vec2;
		kinc_vector3_t vec3;
		kinc_matrix3x3_t mat3;
		kinc_matrix4x4_t mat4;
	};
};

struct varia_graphics_sampler_t
{
	char const * identifier;
	kinc_g4_texture_unit_t location;
	kinc_g4_texture_t texture;
};

struct varia_graphics_material_t
{
	varia_graphics_program_t * program;

	bool compiled;

	vds_array_t<varia_graphics_uniform_t> uniforms; //16 of them
	varia_graphics_uniform_t _uniforms_backing_storage[VARIA_GRAPHICS_MATERIAL_MAX_UNIFORMS + 1]; //Add space for error object

	vds_array_t<varia_graphics_sampler_t> samplers; //8 of them
	varia_graphics_sampler_t _samplers_backing_storage[VARIA_GRAPHICS_MATERIAL_MAX_SAMPLERS + 1];

};

varia_graphics_material_t varia_graphics_material_create_from(varia_graphics_program_t * program);
void varia_graphics_material_compile(varia_graphics_material_t * material);

void varia_graphics_material_add_sampler(varia_graphics_material_t * material, char const * identifier, kinc_g4_texture_t tex);
void varia_graphics_material_add_uniform_float(varia_graphics_material_t * material, char const * identifier, float f);
void varia_graphics_material_add_uniform_vec2(varia_graphics_material_t * material, char const * identifier, vec2s v);
void varia_graphics_material_add_uniform_vec3(varia_graphics_material_t * material, char const * identifier, vec3s v);
void varia_graphics_material_add_uniform_mat3(varia_graphics_material_t * material, char const * identifier, mat3s mat);
void varia_graphics_material_add_uniform_mat4(varia_graphics_material_t * material, char const * identifier, mat4s mat);

void varia_graphics_material_update_sampler(varia_graphics_material_t * material, char const * identifier, kinc_g4_texture_t tex);
void varia_graphics_material_update_uniform_float(varia_graphics_material_t * material, char const * identifier, float f);
void varia_graphics_material_update_uniform_vec2(varia_graphics_material_t * material, char const * identifier, vec2s v);
void varia_graphics_material_update_uniform_vec3(varia_graphics_material_t * material, char const * identifier, vec3s v);
void varia_graphics_material_update_uniform_mat3(varia_graphics_material_t * material, char const * identifier, mat3s mat);
void varia_graphics_material_update_uniform_mat4(varia_graphics_material_t * material, char const * identifier, mat4s mat);


//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||            Default Materials                                        ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

varia_graphics_material_t varia_graphics_material_create_default_textured(kinc_g4_texture_t tex, mat4s mvp);