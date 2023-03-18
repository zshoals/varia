#include "varia/graphics/Program.hpp"
#include "varia/util/Memory.hpp"
#include "kinc/io/filereader.h"
#include "varia/Validation.hpp"
#include "varia/io/Assets.hpp"

static varia_graphics_program_t texture_program;
static bool _initialized = false;

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

	// vds_array_initialize(&texture_program.vertex_structures, varia_memory_get_permanent_allocator(), VARIA_GRAPHICS_PROGRAM_MAX_VERTEX_STRUCTURES);
	vds_array_initialize_direct(&texture_program.vertex_structures, &texture_program._vertex_structure_backing_storage[0], VARIA_GRAPHICS_PROGRAM_MAX_VERTEX_STRUCTURES);

	kinc_g4_vertex_structure_t position;
	kinc_g4_vertex_structure_t color;
	kinc_g4_vertex_structure_t uv;

	kinc_g4_vertex_structure_init(&position);
	kinc_g4_vertex_structure_init(&color);
	kinc_g4_vertex_structure_init(&uv);

	kinc_g4_vertex_structure_add(&position, "vertexPosition", KINC_G4_VERTEX_DATA_F32_3X);
	kinc_g4_vertex_structure_add(&color, "vertexColor", KINC_G4_VERTEX_DATA_U8_4X_NORMALIZED);
	kinc_g4_vertex_structure_add(&uv, "vertexUV", KINC_G4_VERTEX_DATA_F32_2X);

	vds_array_push(&texture_program.vertex_structures, position);
	vds_array_push(&texture_program.vertex_structures, color);
	vds_array_push(&texture_program.vertex_structures, uv);

	kinc_g4_pipeline_init(&texture_program.pipe);
	texture_program.pipe.vertex_shader = &texture_program.vertex_shader;
	texture_program.pipe.fragment_shader = &texture_program.fragment_shader;
	texture_program.pipe.input_layout[0] = &texture_program.vertex_structures[0];
	texture_program.pipe.input_layout[1] = &texture_program.vertex_structures[1];
	texture_program.pipe.input_layout[2] = &texture_program.vertex_structures[2];
	texture_program.pipe.input_layout[3] = NULL;
	kinc_g4_pipeline_compile(&texture_program.pipe);
}

varia_graphics_program_t * varia_graphics_program_get_textured_program(void)
{
	if (_initialized)
	{
		return &texture_program;
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_program:> Tried to retrieve a program, but the subsystem was uninitialized.");
		return nullptr;
	}
}

void varia_graphics_program_initialize_defaults(void)
{
	if (!_initialized)
	{
		_initialized = true;

		//Note(zshoals 03-14-2023):> Actually call all program configurations below this point
		//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
		varia_local_graphics_program_configure_textured();

		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	}
	else
	{
		ENSURE_UNREACHABLE("varia_graphics_program:> Tried to double initialize the program subsystem, which is not allowed.");
	}
}
