#include "varia/graphics/Program.hpp"
#include "varia/util/Memory.hpp"
#include "kinc/io/filereader.h"
#include "varia/Validation.hpp"
#include "varia/io/Assets.hpp"

static varia_graphics_program_textured_t texture_program;

static void varia_local_graphics_program_configure_textured(void)
{
	vds_option_t<kinc_g4_shader> textured_vert = varia_assets_load_vertex_shader("textured-standard.vert");
	vds_option_t<kinc_g4_shader> textured_frag = varia_assets_load_fragment_shader("textured-standard.frag");

	if (textured_vert && textured_frag)
	{
		texture_program.vertex_shader = *textured_vert;
		texture_program.fragment_shader = *textured_frag;
	}
	else
	{
		ENSURE_UNREACHABLE("ERROR: Standard vertex or fragment shader is missing!");
	}

	kinc_g4_vertex_structure_init(&texture_program.position);
	kinc_g4_vertex_structure_init(&texture_program.color);
	kinc_g4_vertex_structure_init(&texture_program.uv);

	kinc_g4_vertex_structure_add(&texture_program.position, "vertexPosition", KINC_G4_VERTEX_DATA_F32_3X);
	kinc_g4_vertex_structure_add(&texture_program.color, "vertexColor", KINC_G4_VERTEX_DATA_F32_2X);
	kinc_g4_vertex_structure_add(&texture_program.uv, "vertexUV", KINC_G4_VERTEX_DATA_U8_4X_NORMALIZED);

	kinc_g4_pipeline_init(&texture_program.pipe);
	texture_program.pipe.vertex_shader = &texture_program.vertex_shader;
	texture_program.pipe.fragment_shader = &texture_program.fragment_shader;
	texture_program.pipe.input_layout[0] = &texture_program.position;
	texture_program.pipe.input_layout[1] = &texture_program.color;
	texture_program.pipe.input_layout[2] = &texture_program.uv;
	texture_program.pipe.input_layout[3] = nullptr;
	kinc_g4_pipeline_compile(&texture_program.pipe);
}


varia_graphics_program_textured_t * varia_graphics_program_get_textured_program(void)
{
	return &texture_program;
}

void varia_graphics_program_initialize_defaults(void)
{
	varia_local_graphics_program_configure_textured();
}
