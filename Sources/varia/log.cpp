
#include "log.hpp"

#include "kinc/log.h"
#include "kinc/system.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

static bool log_in_bounds(varia_stringbuf_t const * buf, size_t chars_to_add)
{
	//Note(zshoals 03-08-2023):> We always leave room for a finalizing null terminator while printing
	return (chars_to_add + buf->current_offset) < buf->capacity - 1;
}

//Generic buffer
void log_print(varia_stringbuf_t * buf)
{
	buf->buffer[buf->current_offset] = '\0';

	if (buf->current_offset == 0)
	{
		return;
	} 

	kinc_log(KINC_LOG_LEVEL_INFO, buf->buffer);
}

void log_clear_buffer(varia_stringbuf_t * buf)
{
	buf->current_offset = 0;
	memset(buf->buffer, 0x69, buf->capacity);

	buf->buffer[0] = '\0';
}

void log_copy_to(varia_stringbuf_t * buf, void * destination, size_t destination_length)
{
	memcpy(destination, &buf->buffer[0], buf->current_offset);
}


void log_string(varia_stringbuf_t * buf, char const * string)
{
	size_t const to_add = strlen(string);

	if (log_in_bounds(buf, to_add))
	{
		int written = sprintf(&buf->buffer[buf->current_offset], string);
		buf->current_offset += written;
	}
}

void log_newline(varia_stringbuf_t * buf)
{
	if (log_in_bounds(buf, 1))
	{
		buf->buffer[buf->current_offset] = '\n';
		++buf->current_offset;
	}
}


void log_float(varia_stringbuf_t * buf, double value)
{
	char temp[128];
	int to_add = snprintf(&temp[0], 128, "%f", value);

	if (log_in_bounds(buf, to_add))
	{
		int written = sprintf(&buf->buffer[buf->current_offset], &temp[0]);
		buf->current_offset += written;
	}
}

void log_int(varia_stringbuf_t * buf, int value)
{
	char temp[128];
	int to_add = snprintf(&temp[0], 128, "%d", value);

	if (log_in_bounds(buf, to_add))
	{
		int written = sprintf(&buf->buffer[buf->current_offset], &temp[0]);
		buf->current_offset += written;
	}
}

void log_int32(varia_stringbuf_t * buf, int32_t value)
{
	char temp[128];
	int to_add = snprintf(&temp[0], 128, "%" PRId32, value);

	if (log_in_bounds(buf, to_add))
	{
		int written = sprintf(&buf->buffer[buf->current_offset], &temp[0]);
		buf->current_offset += written;
	}
}

void log_uint32(varia_stringbuf_t * buf, uint32_t value)
{
	char temp[128];
	int to_add = snprintf(&temp[0], 128, "%" PRIu32, value);

	if (log_in_bounds(buf, to_add))
	{
		int written = sprintf(&buf->buffer[buf->current_offset], &temp[0]);
		buf->current_offset += written;
	}
}

void log_int64(varia_stringbuf_t * buf, int64_t value)
{
	char temp[128];
	int to_add = snprintf(&temp[0], 128, "%" PRId64, value);

	if (log_in_bounds(buf, to_add))
	{
		int written = sprintf(&buf->buffer[buf->current_offset], &temp[0]);
		buf->current_offset += written;
	}
}

void log_uint64(varia_stringbuf_t * buf, uint64_t value)
{
	char temp[128];
	int to_add = snprintf(&temp[0], 128, "%" PRIu64, value);

	if (log_in_bounds(buf, to_add))
	{
		int written = sprintf(&buf->buffer[buf->current_offset], &temp[0]);
		buf->current_offset += written;
	}
}

void log_size(varia_stringbuf_t * buf, size_t value)
{
	char temp[128];
	int to_add = snprintf(&temp[0], 128, "%zu", value);

	if (log_in_bounds(buf, to_add))
	{
		int written = sprintf(&buf->buffer[buf->current_offset], &temp[0]);
		buf->current_offset += written;
	}
}

void log_time(varia_stringbuf_t * buf)
{
	log_float(buf, kinc_time());
}





#define VARIA_G_STRING_BUF_SIZE 100000
static char global_string_buffer_data[VARIA_G_STRING_BUF_SIZE];
static varia_stringbuf_t global_string_buffer;

//"Global" scratch buffer
void Glog_initialize(void)
{
	global_string_buffer.buffer = &global_string_buffer_data[0];
	global_string_buffer.current_offset = 0;
	global_string_buffer.capacity = VARIA_G_STRING_BUF_SIZE;
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








