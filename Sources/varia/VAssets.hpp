#pragma once

#include "varia/VShared.hpp"
#include "varia/ds/VDS-StringBuffer.hpp"
#include "varia/ds/VDS-Arena.hpp"
#include "varia/ds/VDS-Stringmap.hpp"
#include "varia/ds/VDS-Array.hpp"
#include "varia/graphics/VAtlas.hpp"
#include "varia/graphics/VPipeline.hpp"
#include "kinc/graphics4/shader.h"
#include "kinc/graphics4/pipeline.h"

struct Assets
{
    Atlas images;

    Textured_Pipeline texture_painter;

    //TODO(<zshoals> 08-04-2023): How about we not stringly type this and instead
    //use hard instances of shaders that we load up 
    VDS_Stringmap_Storage<kinc_g4_shader_t, 8> vertex_shaders;
    VDS_Stringmap_Storage<kinc_g4_shader_t, 8> fragment_shaders;

    VDS_String_Buffer_Storage<VARIA_MEGABYTES(16)> permanent_strings;
    VDS_String_Buffer_Storage<VARIA_MEGABYTES(8)> temporary_strings;

    VDS_Arena_Storage<VARIA_MEGABYTES(32)> permanent_storage;
    VDS_Arena_Storage<VARIA_MEGABYTES(68)> loading_buffer;
};

Boolean v_assets_load_atlas(Assets * assets, char const * image_path, char const * image_metadata_path);
Boolean v_assets_load_vertex_shader(Assets * assets, char const * name, char const * vertex_path);
Boolean v_assets_load_fragment_shader(Assets * assets, char const * name, char const * fragment_path);