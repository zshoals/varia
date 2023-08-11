#pragma once

#include "varia/VShared.hpp"
#include "varia/VAssetStorage.hpp"
#include "varia/graphics/VPipeline.hpp"
#include "kinc/graphics4/shader.h"
#include "kinc/graphics4/pipeline.h"


struct Asset_Texture
{
    kinc_g4_texture_t texture;
    Boolean loaded;
};

struct Asset_Shader
{
    kinc_g4_shader_t shader;
    kinc_g4_shader_type_t type;
    Boolean loaded;
};

struct Asset_Atlas_Metadata
{
    char const * metadata;
    Integer_64 size;
    Boolean loaded;
};

struct Assets
{
    Asset_Texture atlas_texture_01;
    Asset_Atlas_Metadata atlas_metadata_01;
    Asset_Shader textured_vert;
    Asset_Shader textured_frag;

    // kinc_g4_texture_t atlas_01;
    // kinc_g4_shader_t textured_vert;
    // kinc_g4_shader_t textured_frag;

    //This is in the wrong place as it is not really loaded off the disk directly
    // Textured_Pipeline textured_pipeline;

    //Storages
    Permanent_String_Buffer permanent_strings;
    Temporary_String_Buffer temporary_strings;

    Permanent_Storage_Arena permanent_storage;
    Temporary_Storage_Arena temporary_storage;
};

Boolean v_assets_load_texture(Assets * assets, Asset_Texture * texture, char const * path);
Boolean v_assets_load_shader(Assets * assets, Asset_Shader * shader, char const * path);
Boolean v_assets_load_atlas_metadata(Assets * assets, Asset_Atlas_Metadata * metadata, char const * path);
void v_assets_unload_all(Assets * assets);

// Boolean v_assets_load_pipeline(Asset_Pipeline * pipe, Permanent_Storage_Arena * arena, Asset_Asset_Shader * vert, Asset_Shader * frag)
