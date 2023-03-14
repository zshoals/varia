#include "varia/io/Assets.hpp"

#include "varia/util/Memory.hpp"
#include "kinc/io/filereader.h"
#include "kinc/graphics4/shader.h"

#include "varia/Validation.hpp"

enum class varia_asset_type_e
{
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	TEXTURE,
	SAVE
};

vds_option_t<kinc_g4_shader_t> varia_assets_load_vertex_shader(char const * path)
{
	kinc_file_reader_t reader;
	bool opened = kinc_file_reader_open(&reader, path, KINC_FILE_TYPE_ASSET);
	if (opened)
	{
		vds_allocator_t * perm_alloc = varia_memory_get_permanent_allocator();
		size_t size_in_bytes = kinc_file_reader_size(&reader);
		byte * memory = (byte *)vds_allocator_allocate_aligned_count(perm_alloc, sizeof(byte), size_in_bytes, 1);

		kinc_file_reader_read(&reader, memory, size_in_bytes);
		kinc_file_reader_close(&reader);

		kinc_g4_shader_t vertex_shader;
		kinc_g4_shader_init(&vertex_shader, memory, size_in_bytes, KINC_G4_SHADER_TYPE_VERTEX);

		return vds_option_create_some(vertex_shader);
	}
	else
	{
		return vds_option_create_none<kinc_g4_shader_t>();
	}
}

vds_option_t<kinc_g4_shader_t> varia_assets_load_fragment_shader(char const * path)
{
	kinc_file_reader_t reader;
	bool opened = kinc_file_reader_open(&reader, path, KINC_FILE_TYPE_ASSET);
	if (opened)
	{
		vds_allocator_t * perm_alloc = varia_memory_get_permanent_allocator();
		size_t size_in_bytes = kinc_file_reader_size(&reader);
		byte * memory = (byte *)vds_allocator_allocate_aligned_count(perm_alloc, sizeof(byte), size_in_bytes, 1);

		kinc_file_reader_read(&reader, memory, size_in_bytes);
		kinc_file_reader_close(&reader);

		kinc_g4_shader_t fragment_shader;
		kinc_g4_shader_init(&fragment_shader, memory, size_in_bytes, KINC_G4_SHADER_TYPE_FRAGMENT);

		return vds_option_create_some(fragment_shader);
	}
	else
	{
		return vds_option_create_none<kinc_g4_shader_t>();
	}
}

vds_option_t<kinc_g4_texture_t> varia_assets_load_texture(char const * path)
{
	size_t size_in_bytes = kinc_image_size_from_file(path);
	if (size_in_bytes >= 0) //Note(zshoals 03-14-2023):> The file exists and is not empty
	{
		vds_allocator_t * image_alloc = varia_memory_get_image_allocator();
		byte * memory = (byte *)vds_allocator_allocate_aligned_count(image_alloc, sizeof(byte), size_in_bytes, 16);

		kinc_image_t image;
		kinc_image_init_from_file(&image, memory, path);

		kinc_g4_texture_t texture;
		kinc_g4_texture_init_from_image(&texture, &image);

		varia_memory_reset_image_allocator();

		return vds_option_create_some(texture);
	}
	else
	{
		return vds_option_create_none<kinc_g4_texture_t>();
	}
}
