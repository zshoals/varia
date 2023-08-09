#include "varia/graphics/VAtlas.hpp"

#include "varia/VShared.hpp"

#include "varia/utility/VFileReader.hpp"
#include "varia/utility/VParser.hpp"
#include "varia/utility/VMemset.hpp"
#include "varia/utility/VStringUtil.hpp"

void v_atlas_initialize(Atlas * atlas, Asset_Texture * texture, Asset_Atlas_Metadata const * metadata)
{
    if ( !(texture->loaded & metadata->loaded) ) 
    { 
        return; 
    }

    memset(atlas, 0, sizeof(dereference(atlas)));
    atlas->texture = texture;

    //Process metadata
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Parser parser_storage = ZERO_INIT();
    Parser * parser = address_of(parser_storage);
    v_parser_initialize(parser, static_cast<char const *>(metadata->metadata), metadata->size);

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
}

void v_atlas_destroy(Atlas * atlas)
{
    memset(atlas, 0, sizeof(*atlas));
}

kinc_g4_texture_t * v_atlas_get_texture(Atlas * atlas)
{
    if (atlas->texture->loaded == true)
    {
        return address_of(atlas->texture->texture);
    }
    else
    {
        return nullptr;
    }
}