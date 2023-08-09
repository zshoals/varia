#include "varia/graphics/VGraphicsIR.hpp"

#include "varia/VShared.hpp"

#include "varia/VGamestate.hpp"
#include "kinc/log.h"

void v_graphics_ir_clear(Graphics_Intermediate_Representation * ir)
{
    ir->count = 0;
    vds_array_clear(address_of(ir->items));
    vds_array_clear(address_of(ir->sortables));
}

void v_graphics_ir_build(Graphics_Intermediate_Representation * ir, Gamestate const * visual_world, Atlas const * atlas)
{
    Gamedata const * game = address_of(visual_world->gamedata);
    Atlas_Sub_Image_Array const * subs = const_address_of(atlas->sub_images);

    //TODO(<zshoals> 08-09-2023): We need to do like, not a fixed sized value :)))))
    for_range_var(i, 4)
    {
        VDS_Short_String const * sub_image_name = vds_array_const_location_of(address_of(game->enemy_texture), i);

        //TODO(<zshoals> 08-09-2023): This sucks, this should really be a hash map
        //TODO(<zshoals> 08-09-2023): This could be minimized by not figuring this out on demand, but rather setting the UV
        //  coordinates when we assign a texture instead
        Integer_64 result = vds_array_index_of(subs, [&sub_image_name](Atlas_Sub_Image const * img)
        {
            return vds_string_matches( dereference(sub_image_name), img->name );
        });

        if (result >= 0)
        {
            Atlas_Sub_Image const * sub_image_data = vds_array_const_location_of(subs, result);
            float inverse_size = 1.0f / 4096.0f;

            Graphics_Item * item = vds_array_construct_push(address_of(ir->items));
            item->x     = vds_array_copy_of(address_of(game->x_pos), i);
            item->y     = vds_array_copy_of(address_of(game->y_pos), i);
            item->layer = vds_array_copy_of(address_of(game->layer), i);
            item->w     = vds_array_copy_of(address_of(game->width), i);
            item->h     = vds_array_copy_of(address_of(game->height), i);
            item->u_min = (sub_image_data->origin_x * inverse_size);
            item->v_min = (sub_image_data->origin_y * inverse_size);
            item->u_max = ( (sub_image_data->origin_x + sub_image_data->width) * inverse_size);
            item->v_max = ( (sub_image_data->origin_y + sub_image_data->height) * inverse_size);
            item->r     = 1.0;
            item->g     = 1.0;
            item->b     = 1.0;
            item->a     = 1.0;

            Graphics_Item_Sortable * sortable = vds_array_construct_push(address_of(ir->sortables));
            sortable->layer = item->layer;
            sortable->item = item;

            ir->count += 1;
        }
        else
        {
            kinc_log(KINC_LOG_LEVEL_WARNING, "Graphics IR tried to push an item, but a texture was not found. This item was skipped.");
        }
    }

    //Sort quads based on their "layer", a value that we probably didn't even have to provide at all because we could just use a z value ://////
    //  and the shader would just take care of it ://////
    //TODO(<zshoals> 08-09-2023): It's qsort, this is going to be very slow
    vds_array_sort(address_of(ir->sortables), [](const void * a, const void * b)
    {
        
        Graphics_Item_Sortable const * left = (Graphics_Item_Sortable const *)a;
        Graphics_Item_Sortable const * right = (Graphics_Item_Sortable const *)b;

        return (left->layer < right->layer) ? -1 : 1;
    });
}
