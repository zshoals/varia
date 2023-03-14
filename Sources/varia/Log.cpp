
#include "Log.hpp"

#include "varia/Validation.hpp"

#include "kinc/log.h"
#include "kinc/system.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

// static bool log_in_bounds(varia_stringbuf_t const * buf, size_t chars_to_add)
// {
// 	//Note(zshoals 03-08-2023):> We always leave room for a finalizing null terminator while printing
// 	return (chars_to_add + buf->current_offset) < buf->capacity - 1;
// }

//Generic buffer
void log_print(varia_stringbuf_t * buf)
{
	*vds_dumbbuf_data_at_offset(buf) = '\0';

	i64 const buflen = vds_dumbbuf_length(buf);
	if (buflen == 0)
	{
		return;
	} 

	//Note(zshoals 03-14-2023):> Kinc has an internal print limit of 4096 characters per print
	//Cycle down 
	char * data = vds_dumbbuf_data(buf);
	for (i64 i = 0; i < buflen; i += 4048)
	{
		char tmp[4048];
		memcpy(&tmp, data, 4047);
		tmp[4047] = '\0';

		kinc_log(KINC_LOG_LEVEL_INFO, &tmp[0]);

		data += 4048;
	}
}

void log_clear_buffer(varia_stringbuf_t * buf)
{
	vds_dumbbuf_reset(buf);
	*vds_dumbbuf_data(buf) = '\0';
}

void log_copy_to(varia_stringbuf_t * buf, void * destination, size_t destination_length)
{
	memcpy(destination, &buf->buffer[0], buf->current_offset);
}


void log_string(varia_stringbuf_t * buf, char const * string)
{
	size_t const to_add = strlen(string);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), string);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_newline(varia_stringbuf_t * buf)
{
	if (vds_dumbbuf_can_push_amount(buf, 1))
	{
		*vds_dumbbuf_data_at_offset(buf) = '\n';
		vds_dumbbuf_advance(buf, 1);
	}
}


void log_float(varia_stringbuf_t * buf, double value)
{
	char temp[64];
	int to_add = snprintf(&temp[0], 64, "%f", value);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), &temp[0]);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_double(varia_stringbuf_t * buf, double value)
{
	char temp[64];
	int to_add = snprintf(&temp[0], 64, "%.12f", value);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), &temp[0]);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_int(varia_stringbuf_t * buf, int value)
{
	char temp[64];
	int to_add = snprintf(&temp[0], 64, "%d", value);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), &temp[0]);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_int32(varia_stringbuf_t * buf, int32_t value)
{
	char temp[64];
	int to_add = snprintf(&temp[0], 64, "%" PRId32, value);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), &temp[0]);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_uint32(varia_stringbuf_t * buf, uint32_t value)
{
	char temp[64];
	int to_add = snprintf(&temp[0], 64, "%" PRIu32, value);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), &temp[0]);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_int64(varia_stringbuf_t * buf, int64_t value)
{
	char temp[64];
	int to_add = snprintf(&temp[0], 64, "%" PRId64, value);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), &temp[0]);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_uint64(varia_stringbuf_t * buf, uint64_t value)
{
	char temp[64];
	int to_add = snprintf(&temp[0], 64, "%" PRIu64, value);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), &temp[0]);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_size(varia_stringbuf_t * buf, size_t value)
{
	char temp[64];
	int to_add = snprintf(&temp[0], 64, "%zu", value);

	if (vds_dumbbuf_can_push_amount(buf, to_add))
	{
		int written = sprintf(vds_dumbbuf_data_at_offset(buf), &temp[0]);
		vds_dumbbuf_advance(buf, written);
	}
}

void log_time(varia_stringbuf_t * buf)
{
	log_float(buf, kinc_time());
}





static char global_string_buffer_data[VARIA_G_STRING_BUF_SIZE];
static bool global_string_buffer_initialized = false;
static varia_stringbuf_t global_string_buffer;

//"Global" scratch buffer
void Glog_initialize(void)
{
	vds_dumbbuf_initialize_direct(&global_string_buffer, &global_string_buffer_data[0], VARIA_G_STRING_BUF_SIZE);
	global_string_buffer_initialized = true;
}

varia_stringbuf_t * Glog_reference(void)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	return &global_string_buffer;
}

void Glog_print(void)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_print(&global_string_buffer);
}

void Glog_clear_buffer(void)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_clear_buffer(&global_string_buffer);
}

void Glog_copy_to(void * destination, size_t destination_length)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_copy_to(&global_string_buffer, destination, destination_length);
}

void Glog_string(char const * string)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_string(&global_string_buffer, string);
}

void Glog_newline(void)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_newline(&global_string_buffer);
}

void Glog_float(double value)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_float(&global_string_buffer, value);
}

void Glog_double(double value)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_double(&global_string_buffer, value);
}

void Glog_int(int value)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_int(&global_string_buffer, value);
}

void Glog_int64(int64_t value)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_int64(&global_string_buffer, value);
}

void Glog_uint64(uint64_t value)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_uint64(&global_string_buffer, value);
}

void Glog_int32(int32_t value)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_int32(&global_string_buffer, value);
}

void Glog_uint32(uint32_t value)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_uint32(&global_string_buffer, value);
}

void Glog_size(size_t value)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_size(&global_string_buffer, value);
}

void Glog_time(void)
{
	ENSURE(global_string_buffer_initialized, "Glog must be initialized before logging.");
	log_time(&global_string_buffer);
}








