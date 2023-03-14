#pragma once

#include "varia/ds/Option.hpp"

#include "kinc/image.h"
#include "kinc/graphics4/texture.h"
#include "kinc/graphics4/shader.h"

struct vds_allocator_t;

vds_option_t<kinc_g4_shader_t> varia_assets_load_vertex_shader(char const * path, vds_allocator_t * alloc);
vds_option_t<kinc_g4_shader_t> varia_assets_load_fragment_shader(char const * path, vds_allocator_t * alloc);
vds_option_t<kinc_g4_texture_t> varia_assets_load_texture(char const * path);