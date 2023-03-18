#include "varia/graphics/Graphics.hpp"

#include "varia/ds/Option.hpp"
#include "kinc/graphics4/vertexbuffer.h"


void varia_graphics_initialize(varia_graphics_context_t * graphics)
{
	varia_graphics_program_configure_textured(&graphics->texPainter.textured_program);

	//TODO(zshoals 03-18-2023):> Should this be a fixed size for the vert structure???
	kinc_g4_vertex_buffer_init
	(
		&graphics->texPainter.texPos,
		VARIA_MAX_TEXTURED_VERTEX_DATA,
		&graphics->texPainter.textured_program.vertex_structures[0], //Position
		KINC_G4_USAGE_DYNAMIC,
		0
	);

	kinc_g4_vertex_buffer_init
	(
		&graphics->texPainter.texColor,
		VARIA_MAX_TEXTURED_VERTEX_DATA,
		&graphics->texPainter.textured_program.vertex_structures[1], //Color
		KINC_G4_USAGE_DYNAMIC,
		0
	);

	kinc_g4_vertex_buffer_init
	(
		&graphics->texPainter.texUV,
		VARIA_MAX_TEXTURED_VERTEX_DATA,
		&graphics->texPainter.textured_program.vertex_structures[2], //UV
		KINC_G4_USAGE_DYNAMIC,
		0
	);

	kinc_g4_index_buffer_init
	(
		&graphics->texPainter.texIndex,
		VARIA_MAX_TEXTURED_VERTEX_DATA, //TODO(zshoals 03-18-2023):> Verify if this needs to be smaller?
		KINC_G4_INDEX_BUFFER_FORMAT_32BIT,
		KINC_G4_USAGE_STATIC
	);
}

void varia_graphics_camera_set(varia_graphics_context_t * graphics, mat4s cam)
{

}

void varia_graphics_batch_begin(varia_graphics_context_t * graphics, varia_graphics_material_t mat)
{

}

void varia_graphics_batch_end(varia_graphics_context_t * graphics)
{

}
