
#include "log.hpp"

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

	if (vds_dumbbuf_length(buf) == 0)
	{
		return;
	} 

	kinc_log(KINC_LOG_LEVEL_INFO, vds_dumbbuf_data(buf));
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
static varia_stringbuf_t global_string_buffer;

//"Global" scratch buffer
void Glog_initialize(void)
{
	vds_dumbbuf_initialize_direct(&global_string_buffer, &global_string_buffer_data[0], VARIA_G_STRING_BUF_SIZE);
}

varia_stringbuf_t * Glog_reference(void)
{
	return &global_string_buffer;
}

void Glog_print(void)
{
	log_print(&global_string_buffer);
}

void Glog_clear_buffer(void)
{
	log_clear_buffer(&global_string_buffer);
}

void Glog_copy_to(void * destination, size_t destination_length)
{
	log_copy_to(&global_string_buffer, destination, destination_length);
}

void Glog_string(char const * string)
{
	log_string(&global_string_buffer, string);
}

void Glog_newline(void)
{
	log_newline(&global_string_buffer);
}

void Glog_float(double value)
{
	log_float(&global_string_buffer, value);
}

void Glog_int(int value)
{
	log_int(&global_string_buffer, value);
}

void Glog_int64(int64_t value)
{
	log_int64(&global_string_buffer, value);
}

void Glog_uint64(uint64_t value)
{
	log_uint64(&global_string_buffer, value);
}

void Glog_int32(int32_t value)
{
	log_int32(&global_string_buffer, value);
}

void Glog_uint32(uint32_t value)
{
	log_uint32(&global_string_buffer, value);
}

void Glog_size(size_t value)
{
	log_size(&global_string_buffer, value);
}

void Glog_time(void)
{
	log_time(&global_string_buffer);
}








