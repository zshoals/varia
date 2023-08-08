#pragma once

#include "varia/VShared.hpp"
#include "varia/VAssetStorage.hpp"
#include "varia/graphics/VAtlas.hpp"
#include "varia/graphics/VPipeline.hpp"
#include "kinc/graphics4/shader.h"
#include "kinc/graphics4/pipeline.h"

struct Assets
{
    Atlas atlas;

    kinc_g4_shader_t textured_vert;
    kinc_g4_shader_t textured_frag;
    Textured_Pipeline textured_pipeline;

    Permanent_String_Buffer permanent_strings;
    Temporary_String_Buffer temporary_strings;

    Permanent_Storage_Arena permanent_storage;
    Temporary_Storage_Arena temporary_storage;
};

Boolean v_assets_load_atlas(Assets * assets, char const * image_path, char const * image_metadata_path);
Boolean v_assets_load_default_shaders(Assets * assets);