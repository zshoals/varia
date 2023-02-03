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

		size_t size = kinc_file_reader_size(&reader); //Null terminator
		file.bytes = vds_allocator_malloc(allocator, unsigned char, size + 1);

		int size_read = kinc_file_reader_read(&reader, file.bytes, size);
		++size_read;

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
		size_t size = kinc_file_reader_size(&reader); 
		file.bytes = vds_allocator_malloc(allocator, unsigned char, size + 1); //Add a space for a null term

		//Note(zshoals 02-03-2023):> This size_read value is one less byte than we allocated
		//we allocate an extra byte for the null terminator
		int size_read = kinc_file_reader_read(&reader, file.bytes, size);
		++size_read; //Add the null terminator to the size

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
