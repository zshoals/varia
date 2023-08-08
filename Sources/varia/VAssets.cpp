#include "varia/VAssets.hpp"

#include "varia/VShared.hpp"

#include "varia/VAssetStorage.hpp"
#include "varia/utility/VStringUtil.hpp"
#include "varia/utility/VFileReader.hpp"
#include "varia/graphics/VPipeline.hpp"

#include "kinc/image.h"
#include "kinc/graphics4/texture.h"
#include "kinc/log.h"


Boolean v_assets_load_atlas(Assets * assets, char const * image_path, char const * image_metadata_path)
{
    // VDS_Arena texture_loader_interface = vds_arena_make_interface(address_of(assets->loading_buffer));
    // VDS_Arena metadata_loader_interface = vds_arena_make_interface(address_of(assets->permanent_storage));
    // VDS_String_Buffer sub_image_name_interface = vds_string_buffer_make_interface(address_of(assets->permanent_strings));

    Temporary_Storage_Arena * texture_loader = address_of(assets->temporary_storage);
    Permanent_Storage_Arena * metadata_loader = address_of(assets->permanent_storage);
    Temporary_String_Buffer * sub_image_name = address_of(assets->temporary_strings);

    Boolean loaded = v_atlas_initialize(address_of(assets->images), sub_image_name, texture_loader, metadata_loader, image_path, image_metadata_path);

    if (loaded)
    {
        kinc_log(KINC_LOG_LEVEL_INFO, "Atlas loaded successfully.");
    }
    else
    {
        kinc_log(KINC_LOG_LEVEL_ERROR, "ERROR: Atlas wasn't loaded.");
    }

    return loaded;
}

static inline void v_assets_init_shader(kinc_g4_shader_t * shader, File_Data shader_data, kinc_g4_shader_type_t type)
{
    kinc_g4_shader_init(shader, shader_data.raw_memory, shader_data.requested_size, type);
}

Boolean v_assets_load_default_shaders(Assets * assets)
{
    Permanent_Storage_Arena * perma_storage = address_of(assets->permanent_storage);

    //NOTE(<zshoals> 08-05-2023): We waste some memory here if one loaded but the other didn't
    //  not much though so who cares
    File_Data tex_vert_info = v_filereader_try_load(perma_storage, "textured-standard.vert");
    File_Data tex_frag_info = v_filereader_try_load(perma_storage, "textured-standard.frag");

    if (tex_vert_info.loaded & tex_frag_info.loaded)
    {
        //Vertex
        v_assets_init_shader(address_of(assets->textured_vert), tex_vert_info, KINC_G4_SHADER_TYPE_VERTEX);
        //Fragment
        v_assets_init_shader(address_of(assets->textured_frag), tex_frag_info, KINC_G4_SHADER_TYPE_FRAGMENT);

        kinc_log(KINC_LOG_LEVEL_INFO, "Default Shaders loaded succesfully.");

        return true;
    }
    else
    {
        kinc_log(KINC_LOG_LEVEL_ERROR, "ERROR: Default Shaders weren't loaded.");

        return false;
    }
}