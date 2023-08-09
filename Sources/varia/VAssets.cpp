#include "varia/VAssets.hpp"

#include "varia/VShared.hpp"

#include "varia/VAssetStorage.hpp"
#include "varia/utility/VStringUtil.hpp"
#include "varia/utility/VFileReader.hpp"
#include "varia/utility/VMemset.hpp"

#include "kinc/image.h"
#include "kinc/graphics4/texture.h"
#include "kinc/graphics4/shader.h"
#include "kinc/log.h"

Boolean v_assets_load_shader(Assets * assets, Asset_Shader * shader, char const * path)
{
    if (shader->loaded)
    {
        VARIA_UNREACHABLE("This shader slot was already loaded.");
    }

    kinc_g4_shader_type_t shader_type = KINC_G4_SHADER_TYPE_FRAGMENT;
    
    if (v_string_utility_ends_with(path, ".vert"))
    {
        shader_type = KINC_G4_SHADER_TYPE_VERTEX;
    }
    else if (v_string_utility_ends_with(path, ".frag"))
    {
        shader_type = KINC_G4_SHADER_TYPE_FRAGMENT;
    }
    else
    {
        VARIA_UNREACHABLE("Unknown shader type.");
    }


    Permanent_Storage_Arena * arena = address_of(assets->permanent_storage);
    File_Data file = v_filereader_try_load(arena, path);

    if (file.loaded)
    {
        kinc_g4_shader_init(address_of(shader->shader), file.raw_memory, file.requested_size, shader_type);
        shader->type = shader_type;
        shader->loaded = true;

        return true;
    }
    else
    {
        VARIA_UNREACHABLE("Missing shader.");

        memset(address_of(shader->shader), 0, sizeof(shader->shader));
        shader->type = shader_type;
        shader->loaded = false;

        return false;
    }
}

Boolean v_assets_load_texture(Assets * assets, Asset_Texture * texture, char const * path)
{
    if (texture->loaded)
    {
        VARIA_UNREACHABLE("This texture slot was already loaded.");
    }

    Temporary_Storage_Arena * arena = address_of(assets->temporary_storage);
    vds_arena_clear(arena);

    File_Data file = v_filereader_try_load(arena, path);

    if (file.loaded)
    {
        Integer_64 real_size = kinc_image_size_from_encoded_bytes(file.raw_memory, file.requested_size, "k");
        void * destination_memory = vds_arena_allocate(arena, real_size);

        kinc_image_t image;
        kinc_image_init_from_encoded_bytes(address_of(image), destination_memory, file.raw_memory, file.requested_size, "k");

        kinc_g4_texture_init_from_image(address_of(texture->texture), address_of(image));

        kinc_image_destroy(address_of(image));
        vds_arena_clear(arena);

        texture->loaded = true;

        return true;
    }
    else
    {
        texture->loaded = false;

        return false;
    }
}

Boolean v_assets_load_atlas_metadata(Assets * assets, Asset_Atlas_Metadata * metadata, char const * path)
{
    if (metadata->loaded)
    {
        VARIA_UNREACHABLE("This atlas metadata was already loaded.");
    }

    Permanent_Storage_Arena * arena = address_of(assets->permanent_storage);
    File_Data file = v_filereader_try_load(arena, path);

    if (file.loaded)
    {
        metadata->metadata = static_cast<char const *>(file.raw_memory);
        metadata->size = file.requested_size;
        metadata->loaded = true;

        return true;
    }
    else
    {
        metadata->metadata = nullptr;
        metadata->size = 0;
        metadata->loaded = false;

        return false;
    }
}