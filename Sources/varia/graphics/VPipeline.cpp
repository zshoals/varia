#include "varia/graphics/VPipeline.hpp"

#include "varia/VShared.hpp"

#include "kinc/graphics4/vertexstructure.h"
#include "kinc/graphics4/pipeline.h"
#include "kinc/graphics4/graphics.h"
#include "kinc/system.h"

static inline void v_internal_pipeline_textured_update_callback(Textured_Pipeline * pipe, kinc_g4_texture_t * texture)
{
    //Replace the old pipeline with our new one yay
    kinc_g4_set_pipeline(address_of(pipe->pipe));

    //Update onboard uniforms
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        pipe->time = (float)kinc_time();
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Update constant locations and texture locations
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        kinc_g4_set_float(pipe->time_location, pipe->time);

        //NOTE(<zshoals> 08-07-2023): We only have one single texture/texturearray because
        //  we are GIGABRAINED and loading everything at startup.
        //  So, just set the texture on pipeline change since there's only one anyway and forget about it
        kinc_g4_set_texture(pipe->texture_unit, texture);
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}

void v_pipeline_initialize_textured(Textured_Pipeline * pipe, kinc_g4_vertex_structure_t * vs, kinc_g4_shader_t * vertex_shader, kinc_g4_shader_t * fragment_shader)
{
    kinc_g4_pipeline_init(address_of(pipe->pipe));

    pipe->pipe.input_layout[0] = vs;
    pipe->pipe.input_layout[1] = NULL;
    pipe->pipe.vertex_shader = vertex_shader;
    pipe->pipe.fragment_shader = fragment_shader;

    pipe->pipe.blend_source = KINC_G4_BLEND_ONE;
    pipe->pipe.blend_destination = KINC_G4_BLEND_ZERO;

    kinc_g4_pipeline_compile(address_of(pipe->pipe));

    pipe->time_location = kinc_g4_pipeline_get_constant_location(address_of(pipe->pipe), "vTime");
    pipe->texture_unit = kinc_g4_pipeline_get_texture_unit(address_of(pipe->pipe), "tex");

    pipe->update_callback = &v_internal_pipeline_textured_update_callback;
}