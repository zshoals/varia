#pragma once

#include "varia/VShared.hpp"
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

    // VDS_Array_Storage<Shader, 8> shaders;
    // VDS_Array_Storage<kinc_g4_pipeline_t, 8> pipelines;

    VDS_Stringmap_Storage<kinc_g4_shader_t, 8> vertex_shaders;
    VDS_Stringmap_Storage<kinc_g4_shader_t, 8> fragment_shaders;
    VDS_Stringmap_Storage<Graphics_Pipeline, 8> pipelines;

    String_Buffer<VARIA_MEGABYTES(16)> permanent_strings;
    String_Buffer<VARIA_MEGABYTES(8)> temporary_strings;

    VDS_Arena_Storage<VARIA_MEGABYTES(32)> permanent_storage;
    VDS_Arena_Storage<VARIA_MEGABYTES(68)> loading_buffer;
};

Boolean v_assets_load_atlas(Assets * assets, char const * image_path, char const * image_metadata_path);
Boolean v_assets_load_vertex_shader(Assets * assets, char const * name, char const * vertex_path);
Boolean v_assets_load_fragment_shader(Assets * assets, char const * name, char const * fragment_path);
Boolean v_assets_load_pipeline_normal_textured(Assets * assets, char const * vertex_shader, char const * fragment_shader);