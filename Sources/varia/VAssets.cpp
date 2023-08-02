#include "varia/VAssets.hpp"

#include "varia/VShared.hpp"
#include "varia/utility/VStringUtil.hpp"
#include "kinc/io/filereader.h"


static void * v_assets_allocate(Assets * assets, Integer_64 requested_size)
{
    VDS_Arena arena_interface = vds_arena_make_interface(address_of(assets->permanent_storage));
    VDS_Arena * arena = address_of(arena_interface);

    return vds_arena_allocate(arena, requested_size);
}

// void v_assets_initialize(Assets * assets)
// {
//     //TODO(<zshoals> 07-31-2023): Something? Maybe nothing.
//     // VDS_Array<Vertex_Shader> vertex_shaders = vds_array_make_interface(address_of(assets->vertex_shaders));
//     // VDS_Array<Vertex_Shader> * vertex_shaders_location = address_of(vertex_shaders);
// }


Boolean v_assets_load_atlas(Assets * assets, char const * image_path, char const * image_metadata_path)
{
    kinc_file_reader_t image_reader;
    kinc_file_reader_t image_metadata_reader;

    Boolean image_opened = kinc_file_reader_open(address_of(image_reader), image_path, KINC_FILE_TYPE_ASSET);
    Boolean image_metadata_opened = kinc_file_reader_open(address_of(image_metadata_reader), image_metadata_path, KINC_FILE_TYPE_ASSET);

    Integer_64 image_file_size = 0;
    Integer_64 image_metadata_file_size = 0;

    if (image_opened && image_metadata_opened)
    {
        image_file_size = kinc_file_reader_size(address_of(image_reader));
        image_metadata_file_size = kinc_file_reader_size(address_of(image_metadata_reader));

        assets->images.raw_image_data = v_assets_allocate(assets, image_file_size);
        assets->images.raw_metadata = v_assets_allocate(assets, image_metadata_file_size);

        kinc_file_reader_read(address_of(image_reader), assets->images.raw_image_data, image_file_size);
        kinc_file_reader_read(address_of(image_metadata_reader), assets->images.raw_metadata, image_metadata_file_size);

        kinc_file_reader_close(address_of(image_reader));
        kinc_file_reader_close(address_of(image_metadata_reader));
        
        //TODO(<zshoals> 08-02-2023): Handle metadata reading here

        assets->images.width = 4096;
        assets->images.height = 4096;

        //Process images
        {
            char const * as_string = static_cast<char const *>(assets->images.raw_metadata);
            Integer_64 head = 0;
            //Read and ignore atlas name (unused);
            while ( (as_string[head] != '\n') && (head < image_metadata_file_size))
            {
                head += 1;
            }
            head += 1;
            //Read and ignore atlas size (always 4096 x 4096)
            while (as_string[head] != '\n' && (head < image_metadata_file_size))
            {
                head += 1;
            }
            head += 1;

            //Begin reading subimages
            while (head < image_metadata_file_size)
            {
                //Read the image name
                Integer_64 perma_string_length = 0;
                while (as_string[head] != '\n' && (head < image_metadata_file_size))
                {
                    perma_string_length += 1;
                    head += 1;
                }

                char const * line_start = address_of(as_string[head - perma_string_length]);
                head += 1;

                //Skip "bounds:" text
                while (!v_string_utility_is_numeric(as_string[head]) && (head < image_metadata_file_size))
                {
                    head += 1;
                }
                
                //Read 1 of 4 values
                Integer_64 string_length = 0;
                while (v_string_utility_is_numeric(as_string[head]) && (head < image_metadata_file_size))
                {
                    string_length += 1;
                    head += 1;
                }
                Integer_64 origin_x = v_string_utility_string_as_int(address_of(as_string[head - string_length]), string_length);

                //Skip a comma, read value 2
                head += 1;

                //Read 2 of 4 values
                string_length = 0;
                while (v_string_utility_is_numeric(as_string[head]) && (head < image_metadata_file_size))
                {
                    string_length += 1;
                    head += 1;
                }
                Integer_64 origin_y = v_string_utility_string_as_int(address_of(as_string[head - string_length]), string_length);

                //Skip a comma, read value 3
                head += 1;

                //Read 3 of 4 values
                string_length = 0;
                while (v_string_utility_is_numeric(as_string[head]) && (head < image_metadata_file_size))
                {
                    string_length += 1;
                    head += 1;
                }
                Integer_64 width = v_string_utility_string_as_int(address_of(as_string[head - string_length]), string_length);

                //Skip a comma, read value 4
                head += 1;

                //Read 4 of 4 values
                string_length = 0;
                while (v_string_utility_is_numeric(as_string[head]) && (head < image_metadata_file_size))
                {
                    string_length += 1;
                    head += 1;
                }
                Integer_64 height = v_string_utility_string_as_int(address_of(as_string[head - string_length]), string_length);

                //Advance to next line, skipping \r\n
                head += 2;

                String_Buffer_Reference sub_image_name = v_string_buffer_emplace_string(address_of(assets->permanent_strings), line_start, perma_string_length);

                VDS_Array<Atlas_Sub_Image> array_interface = vds_array_make_interface(address_of(assets->images.sub_images));
                VDS_Array<Atlas_Sub_Image> * array = address_of(array_interface);

                Atlas_Sub_Image * sub_image = vds_array_construct_push(array);
                {
                    sub_image->image_name = sub_image_name;
                    sub_image->origin_x = (Integer_16)origin_x;
                    sub_image->origin_y = (Integer_16)origin_y;
                    sub_image->width    = (Integer_16)width;
                    sub_image->height   = (Integer_16)height;
                }

            }

        }


        return true;
    }
    else
    {
        if (image_opened)
        {
            kinc_file_reader_close(address_of(image_reader));
        }

        if (image_metadata_opened)
        {
            kinc_file_reader_close(address_of(image_metadata_reader));
        }

        return false;
    }
}
