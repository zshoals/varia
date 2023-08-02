// #pragma once

#include "varia/VShared.hpp"
#include "varia/utility/VAtlas.hpp"
#include "kinc/graphics4/shader.h"
#include "varia/ds/VDS-Arena.hpp"

struct Assets
{

    // Vertex_Shader textured_vert;
    // Fragment_Shader textured_frag;

    // Vertex_Shader random_vert;
    // Fragment_Shader random_frag;

    // Pipeline blend_normal_textured;
    // Pipeline blend_normal_random;

    Atlas images;

    String_Buffer<VARIA_MEGABYTES(16)> permanent_strings;
    String_Buffer<VARIA_MEGABYTES(8)> temporary_strings;

    VDS_Arena_Storage<VARIA_MEGABYTES(64)> permanent_storage;
};

// void v_assets_initialize(Assets * assets);
Boolean v_assets_load_atlas(Assets * assets, char const * image_path, char const * image_metadata_path);