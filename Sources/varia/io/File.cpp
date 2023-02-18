#include "File.hpp"

#include "varia/ds/Allocator.hpp"
#include "kinc/io/filereader.h"

//TODO(zshoals 02-17-2023):> This should have a size limit parameter passed in
static inline vds_result_t<varia_io_file_t> vds_io_file_unitlocal_load_file(const char * filepath, vds_allocator_t * allocator, int filetype)
{
	kinc_file_reader_t reader = {};
	bool opened = kinc_file_reader_open(&reader, filepath, filetype);

	varia_io_file_t file;
	{
		file.bytes = nullptr;
		file.size = 0;
		file.loaded = false;
	}

	vds_result_t<varia_io_file_t> res;

	if (opened)
	{

		size_t size = kinc_file_reader_size(&reader);
		file.bytes = vds_allocator_malloc(allocator, unsigned char, size);

		int size_read = kinc_file_reader_read(&reader, file.bytes, size);

		file.size = size_read;
		file.loaded = true;

		res.valid = VDS_RESULT_STATUS_SUCCESS_E;

	}
	else
	{
		res.valid = VDS_RESULT_STATUS_FAILURE_E;
	}

	res.value = file;
	return res;
}

//TODO(zshoals 02-17-2023):> This should have a size limit parameter passed in
vds_result_t<varia_io_file_t> varia_io_file_load_asset(const char * filepath, vds_allocator_t * allocator)
{
	return vds_io_file_unitlocal_load_file(filepath, allocator, KINC_FILE_TYPE_ASSET);
}

//TODO(zshoals 02-17-2023):> This should have a size limit parameter passed in
vds_result_t<varia_io_file_t> varia_io_file_load_save(const char * filepath, vds_allocator_t * allocator)
{
	return vds_io_file_unitlocal_load_file(filepath, allocator, KINC_FILE_TYPE_SAVE);
}
