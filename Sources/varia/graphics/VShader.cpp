#include "varia/graphics/VShader.hpp"
#include "kinc/io/filereader.h"

Boolean v_shader_initialize(Graphics_Shader * shader, char const * name, char const * vertex_shader_path, char const * fragment_shader_path, VDS_Arena * arena)
{
    shader->name = name;

    kinc_file_reader_t vertex_reader;
    Boolean vertex_opened = kinc_file_reader_open(address_of(vertex_reader), vertex_shader_path, KINC_FILE_TYPE_ASSET);

    kinc_file_reader_t fragment_reader;
    Boolean fragment_opened = kinc_file_reader_open(address_of(fragment_reader), fragment_shader_path, KINC_FILE_TYPE_ASSET);

    Boolean shaders_loaded = vertex_opened & fragment_opened;

    if (shaders_loaded)
    {
        //Vertex shader
        {
            Integer_64 requested_size = kinc_file_reader_size(address_of(vertex_reader));
            void * raw_memory = vds_arena_allocate(arena, requested_size);
            kinc_g4_shader_init(address_of(shader->vertex), raw_memory, requested_size, KINC_G4_SHADER_TYPE_VERTEX);
        }

        //Fragment shader
        {
            Integer_64 requested_size = kinc_file_reader_size(address_of(fragment_reader));
            void * raw_memory = vds_arena_allocate(arena, requested_size);
            kinc_g4_shader_init(address_of(shader->fragment), raw_memory, requested_size, KINC_G4_SHADER_TYPE_VERTEX);
        }
    }

    //Cleanup
    if (vertex_opened)
    {
        kinc_file_reader_close(address_of(vertex_reader));
    }

    if (fragment_opened)
    {
        kinc_file_reader_close(address_of(fragment_reader));
    }

    if (shaders_loaded)
    {
        return true;
    }
    else
    {
        return false;
    }
}