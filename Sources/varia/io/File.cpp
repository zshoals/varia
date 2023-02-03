#include "File.hpp"

#include "varia/ds/Allocator.hpp"
#include "kinc/io/filereader.h"

vds_result_t<varia_io_file_t> varia_io_file_load_asset(const char * filepath, vds_allocator_t * allocator)
{
	kinc_file_reader_t reader = {};
	bool opened = kinc_file_reader_open(&reader, filepath, KINC_FILE_TYPE_ASSET);

	varia_io_file_t file;
	{
		file.bytes = nullptr;
		file.size = 0;
		file.loaded = false;
	}

	vds_result_t<varia_io_file_t> res;

	if (opened)
	{

		size_t size = kinc_file_reader_size(&reader) + 1; //Null terminator
		file.bytes = vds_allocator_malloc(allocator, unsigned char, size);

		int size_read = kinc_file_reader_read(&reader, file.bytes, size - 1);

		file.size = size_read;
		file.loaded = true;
		file.bytes[size] = '\0';

		res.valid = VDS_RESULT_STATUS_SUCCESS_E;

	}
	else
	{
		res.valid = VDS_RESULT_STATUS_FAILURE_E;
	}

	res.value = file;
	return res;
}

vds_result_t<varia_io_file_t> varia_io_file_load_save(const char * filepath, vds_allocator_t * allocator)
{
	kinc_file_reader_t reader = {};
	bool opened = kinc_file_reader_open(&reader, filepath, KINC_FILE_TYPE_SAVE);

	varia_io_file_t file;
	{
		file.bytes = nullptr;
		file.size = 0;
		file.loaded = false;
	}

	vds_result_t<varia_io_file_t> res;

	if (opened)
	{
		size_t size = kinc_file_reader_size(&reader) + 1; // Null terminator
		file.bytes = vds_allocator_malloc(allocator, unsigned char, size);

		int size_read = kinc_file_reader_read(&reader, file.bytes, size - 1);

		file.size = size_read;
		file.loaded = true;
		file.bytes[size_read] = '\0';

		res.valid = VDS_RESULT_STATUS_SUCCESS_E;

	}
	else
	{
		res.valid = VDS_RESULT_STATUS_FAILURE_E;
	}

	res.value = file;
	return res;

}
