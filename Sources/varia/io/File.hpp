#pragma once

#include "varia/ds/Result.hpp"
#include "varia/ds/StringView.hpp"

struct varia_io_file_t
{
	unsigned char * bytes;
	size_t size;
	bool loaded;
};

vds_result_t<varia_io_file_t> varia_io_file_load_asset(const char * filepath, unsigned char * buffer);
vds_result_t<varia_io_file_t> varia_io_file_load_save(const char * filepath, unsigned char * buffer);

template<int MaxSize>
vds_strview_sequence_t<MaxSize> varia_io_file_read_lines(varia_io_file_t file)
{
	char const * text = (char const *)(file.bytes);

	char proxy[1];
	proxy[0] = '\n';
	char const * splitter = &proxy[0];

	return vds_strview_split_by<MaxSize>(vds_strview_create(text), splitter);
}