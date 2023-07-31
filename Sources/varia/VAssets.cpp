#include "varia/VAssets.hpp"

#include "varia/VShared.hpp"
#include "varia/ds/VDS-Buffer.hpp"
#include "kinc/io/filereader.h"


void v_assets_initialize(Assets * assets)
{
    //TODO(<zshoals> 07-31-2023): Something? Maybe nothing.
}

Boolean v_assets_load_asset(Assets * assets, char const * asset_path, E_Asset_Type asset_type)
{
    Boolean loaded_successfully = false;

    switch (asset_type)
    {
        default:
        {
            VARIA_UNREACHABLE("Attempted to load unhandled asset type.");
            break;
        }
        case E_Asset_Type::Texture:
        {
            break;
        }
        case E_Asset_Type::Vertex_Shader:
        {
            //TODO(<zshoals> 07-31-2023): Terrible hack: remove immediately
            //  how do we transfer this data (or any for any asset) into this asset loader bs?
            Vertex_Shader shader = ZERO_INIT();
            {
                shader.name = "standard_vert";
                shader.path = "textured-standard.vert";
                shader.shader_type = KINC_G4_SHADER_TYPE_VERTEX;
            }

            VDS_Buffer buf = vds_buffer_make_interface(address_of(shader.data));
            VDS_Buffer * buf_loc = address_of(buf);
            loaded_successfully = vds_buffer_read_asset_from_file(buf_loc, shader.path);
            if (loaded_successfully)
            {
                VDS_Array<Vertex_Shader> shaders = vds_array_make_interface(address_of(assets->vertex_shaders));
                VDS_Array<Vertex_Shader> * shaders_location = address_of(shaders);

                if (vds_array_can_push(shaders_location))
                {
                    vds_array_push(shaders_location, shader);
                }
            }
            else
            {
                VARIA_UNREACHABLE("Tried to load this thing but it blew up, RIP");
            }
            break;
        }
        case E_Asset_Type::Fragment_Shader:
        {
            break;
        }
    }

    return loaded_successfully;
}