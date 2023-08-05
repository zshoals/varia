#pragma once

#include "varia/VShared.hpp"
#include "varia/ds/VDS-StringBuffer.hpp"
#include "varia/ds/VDS-Array.hpp"
#include "varia/ds/VDS-Arena.hpp"
#include "kinc/graphics4/texture.h"

struct Atlas_Sub_Image
{
    VDS_String_Buffer_Reference image_name;

    Integer_16 origin_x;
    Integer_16 origin_y;
    Integer_16 width;
    Integer_16 height;
    Integer_8 atlas_index;
};

struct Atlas
{
    kinc_g4_texture_t texture;

    Integer_16 width;
    Integer_16 height;

    //NOTE(<zshoals> 08-02-2023): Sub images are cheap, 4096 of them is only 131kb
    VDS_Array_Storage<Atlas_Sub_Image, 4096> sub_images;
};

Boolean v_atlas_initialize(Atlas * atlas, VDS_String_Buffer * sb, VDS_Arena * image_arena, VDS_Arena * metadata_arena, char const * image_path, char const * metadata_path);
void v_atlas_destroy(Atlas * atlas);