#pragma once

#include "varia/VShared.hpp"
#include "varia/utility/VStringBuffer.hpp"
#include "varia/ds/VDS-Array.hpp"

struct Atlas_Sub_Image
{
    String_Buffer_Reference image_name;

    Integer_16 origin_x;
    Integer_16 origin_y;
    Integer_16 width;
    Integer_16 height;
};

struct Atlas
{
    Integer_16 width;
    Integer_16 height;

    VDS_Array_Storage<Atlas_Sub_Image, 2048> sub_images;

    //TODO(<zshoals> 08-02-2023): We don't really need to consume image memory
    //  after we've loaded it as a texture...
    void * raw_image_data;
    void * raw_metadata;
};
