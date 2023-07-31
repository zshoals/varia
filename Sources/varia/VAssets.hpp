#pragma once

#include "varia/VShared.hpp"
#include "varia/ds/VDS-Array.hpp"
#include "varia/ds/VDS-Buffer.hpp"
#include "kinc/graphics4/shader.h"

enum class E_Asset_Type
{
    Blob,
    Texture,
    Vertex_Shader,
    Fragment_Shader,
    Sound_Effect,
    Music,
};

struct Vertex_Shader
{
    char const * name;
    char const * path;
    kinc_g4_shader_type_t shader_type;
    VDS_Buffer_Storage<VARIA_KILOBYTES(32)> data;
};

struct Fragment_Shader
{
    char const * name;
    char const * path;
    kinc_g4_shader_type_t shader_type;
    VDS_Buffer_Storage<VARIA_KILOBYTES(32)> data;
};

struct Assets
{
    VDS_Array_Storage<Vertex_Shader, 8> vertex_shaders;
    VDS_Array_Storage<Fragment_Shader, 8> fragment_shaders;

    VDS_Buffer_Storage<68 * 1000 * 1000> image_loading_buffer; //68mb
    VDS_Buffer_Storage<20 * 1000 * 1000> file_loading_buffer; //20mb
};

void v_assets_initialize(Assets * assets);
Boolean v_assets_load_asset(Assets * assets, char const * asset_path, E_Asset_Type asset_type);