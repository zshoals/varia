#pragma once

#include "varia/VShared.hpp"

#include "varia/VAssets.hpp"
#include "varia/ds/VDS-StringBuffer.hpp"
#include "varia/ds/VDS-String.hpp"
#include "varia/ds/VDS-Array.hpp"
#include "varia/ds/VDS-Arena.hpp"
#include "kinc/graphics4/texture.h"

struct Atlas_Sub_Image
{
    VDS_Short_String name;

    Integer_16 origin_x;
    Integer_16 origin_y;
    Integer_16 width;
    Integer_16 height;
    Integer_8 atlas_index;
};

using Atlas_Sub_Image_Array = VDS_Array<Atlas_Sub_Image, 4096>;

struct Atlas
{
    Asset_Texture * texture;

    Integer_16 width;
    Integer_16 height;

    //NOTE(<zshoals> 08-02-2023): Sub images are cheap, 4096 of them is only 131kb
    Atlas_Sub_Image_Array sub_images;
};

// Boolean v_atlas_initialize(Atlas * atlas, Temporary_String_Buffer * sb, Temporary_Storage_Arena * image_arena, Permanent_Storage_Arena * metadata_arena, char const * image_path, char const * metadata_path);
void v_atlas_initialize(Atlas * atlas, Asset_Texture * texture, Asset_Atlas_Metadata const * metadata);
kinc_g4_texture_t * v_atlas_get_texture(Atlas * atlas);
VDS_Result<Atlas_Sub_Image const *> v_atlas_find_sub_image(Atlas const * atlas, VDS_Short_String const * image);
void v_atlas_destroy(Atlas * atlas);