#pragma once

#include "varia/ds/Result.hpp"
#include "varia/ds/StringView.hpp"

struct vds_allocator_t;

struct varia_io_file_t
{
	unsigned char * bytes;
	size_t size;
	bool loaded;
};

vds_result_t<varia_io_file_t> varia_io_file_load_asset(const char * filepath, vds_allocator_t * allocator);
vds_result_t<varia_io_file_t> varia_io_file_load_save(const char * filepath, vds_allocator_t * allocator);

template<int MaxSize>
vds_strview_sequence_t<MaxSize> varia_io_file_read_lines(varia_io_file_t file)
{
	char const * text = (char const *)(file.bytes);

	char proxy[1];
	proxy[0] = '\n';
	char const * splitter = &proxy[0];

	//TODO(zshoals 02-02-2023):> Strings from the config file aren't actually null terminated
	//so that doesn't work, we need to count characters
	vds_strview_sequence_t<MaxSize> dummy = vds_strview_split_by_v2<MaxSize>(vds_strview_create_with_length(text, file.size), splitter);
	return dummy;
}