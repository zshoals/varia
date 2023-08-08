#include "varia/graphics/VAtlas.hpp"

#include "varia/VShared.hpp"

#include "varia/VAssetStorage.hpp"
#include "varia/utility/VFileReader.hpp"
#include "varia/utility/VParser.hpp"
#include "varia/utility/VMemset.hpp"
#include "varia/utility/VStringUtil.hpp"

#include "kinc/image.h"
#include "kinc/graphics4/texture.h"

Boolean v_atlas_initialize(Atlas * atlas, Temporary_String_Buffer * sb, Temporary_Storage_Arena * image_arena, Permanent_Storage_Arena * metadata_arena, char const * image_path, char const * metadata_path)
{
    if (!v_string_utility_ends_with(image_path, ".k"))
    {
        //NOTE(<zshoals> 08-05-2023): Right now, we only accept k files with LZ4 compression
        //  these files load fast and it's a good habit to use them T B Q H
        VARIA_UNREACHABLE("Currently, only .k LZ4 image files are supported.");
        return false;
    }

    //Attempt to upload and initialize the atlas image and metadata
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    memset(atlas, 0, sizeof(*atlas));

    Boolean image_can_load = v_filereader_can_open(image_path);
    Boolean metadata_can_load = v_filereader_can_open(metadata_path);

    if ( !(image_can_load & metadata_can_load) ) { return false; }

    File_Data image_info = v_filereader_try_load(image_arena, image_path);
    File_Data metadata_info = v_filereader_try_load(metadata_arena, metadata_path);

    if ( !(image_info.loaded & metadata_info.loaded) ) 
    { 
        VARIA_ASSERT(0, "Somehow we didn't load the atlas data even though we checked it beforehand."
            " Operating system issues?");

        return false;
    }

    kinc_image_t image = ZERO_INIT();
    Integer_64 image_decoded_size = kinc_image_size_from_encoded_bytes(image_info.raw_memory, image_info.requested_size, "k");
    void * image_decoded_memory = vds_arena_allocate(image_arena, image_decoded_size);
    kinc_image_init_from_encoded_bytes(address_of(image), image_decoded_memory, image_info.raw_memory, image_decoded_size, "k");

    //NOTE(<zshoals> 08-07-2023): ACTUALLY LOAD THE TEXTURE (ugh...)
    {
        kinc_g4_texture_init_from_image(address_of(atlas->texture), address_of(image));
    }

    kinc_image_destroy(address_of(image));
    //NOTE(<zshoals> 08-04-2023): Safe to reset the temporary arena now if desired

    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::




    //Process metadata
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Parser parser_storage = ZERO_INIT();
    Parser * parser = address_of(parser_storage);
    v_parser_initialize(parser, static_cast<char const *>(metadata_info.raw_memory), metadata_info.requested_size);

    //Skip past the atlas name (we don't need it)
    v_parser_move_to_next_line(parser);

    //Skip past the atlas sizing info (always 4096x4096)
    atlas->width = 4096;
    atlas->height = 4096;
    v_parser_move_to_next_line(parser);

    //Process all subimage metadata
    //TODO(<zshoals> 08-05-2023): Replace with stringmap
    Atlas_Sub_Image_Array * sub_images = address_of(atlas->sub_images);

    while (v_parser_has_lines(parser))
    {
        //Get the sub_image name
        Atlas_Sub_Image * sub_image = vds_array_construct_push(sub_images);

        Parser_Line_Info info = v_parser_read_line(parser);
        sub_image->name = vds_string_create_with_length<VDS_Short_String>(info.line_data, (Integer_32)info.length);

        //Move to the first sub_image data packet, probably "bounds:"
        v_parser_move_to_next_line(parser);

        if (v_parser_line_starts_with(parser, "bounds:"))
        {
            //Skip "bounds:" and get the x origin
            v_parser_move_to_next_integer(parser);
            sub_image->origin_x = (Integer_16)v_parser_read_integer(parser);

            //y origin
            v_parser_move_to_next_integer(parser);
            sub_image->origin_y = (Integer_16)v_parser_read_integer(parser);

            //width
            v_parser_move_to_next_integer(parser);
            sub_image->width = (Integer_16)v_parser_read_integer(parser);

            //height
            v_parser_move_to_next_integer(parser);
            sub_image->height = (Integer_16)v_parser_read_integer(parser);

            //TODO(<zshoals> 08-05-2023): We default to a zero index but we may want to change this
            //  if we start using texture arrays
            sub_image->atlas_index = 0;
        }
        else
        {
            //There was somehow no associated data with the image, so rewind the sub_image storage
            //  that we initially retrieved
            vds_array_pop(sub_images);
        }

        //next sub_image
        v_parser_move_to_next_line(parser);
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return true;
}

void v_atlas_destroy(Atlas * atlas)
{
    kinc_g4_texture_destroy(address_of(atlas->texture));
    memset(atlas, 0, sizeof(*atlas));
}