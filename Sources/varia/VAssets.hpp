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

    kinc_g4_shader_t textured_vert;
    kinc_g4_shader_t textured_frag;
    Textured_Pipeline textured_pipeline;

    VDS_String_Buffer_Storage<VARIA_MEGABYTES(16)> permanent_strings;
    VDS_String_Buffer_Storage<VARIA_MEGABYTES(8)> temporary_strings;

    VDS_Arena_Storage<VARIA_MEGABYTES(32)> permanent_storage;
    VDS_Arena_Storage<VARIA_MEGABYTES(160)> loading_buffer;
};

Boolean v_assets_load_atlas(Assets * assets, char const * image_path, char const * image_metadata_path);
Boolean v_assets_load_default_shaders(Assets * assets);