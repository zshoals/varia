#pragma once

#include "varia/VShared.hpp"
#include "varia/ds/VDS-Arena.hpp"
#include "kinc/graphics4/shader.h"
#include "kinc/io/filereader.h"

struct Graphics_Shader
{
    char const * name;
    kinc_g4_shader_t vertex;
    kinc_g4_shader_t fragment;
};

Boolean v_shader_initialize(Graphics_Shader * shader, char const * name, char const * vertex_shader_path, char const * fragment_shader_path, VDS_Arena * arena);
