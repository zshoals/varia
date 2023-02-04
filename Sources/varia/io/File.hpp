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
