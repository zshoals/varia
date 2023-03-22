#pragma once

#include "varia/ds/Allocator.hpp"
#include "varia/Validation.hpp"

#include <stdint.h>
#include <string.h>

struct vds_string_t
{
	char const * _str;
	int64_t _len;
};

static inline vds_string_t vds_string_create(char const * string, vds_allocator_t * alloc)
{
	int64_t byte_len = strlen(string) + 1;
	char * write_location = (char *)vds_allocator_allocate_aligned(alloc, byte_len, 1);
	memcpy(write_location, string, byte_len);

	vds_string_t out;
	out._len = byte_len - 1;
	out._str = write_location;

	return out;
}

static inline vds_string_t vds_string_create_range(char const * string, int64_t low, int64_t high, vds_allocator_t * alloc)
{
	DEBUG_ENSURE(low >= 0, "vds_string_t:> Low range must be 0 or higher.");

	int64_t str_len = strlen(string);
	if (high > str_len) { high = str_len; }

	int64_t copy_amount = high - low;
	str_len -= (str_len - copy_amount);

	//Note(zshoals 03-21-2023):> Make sure there's a null terminator, and
	//make sure that the pointer we pass into memcpy does not go out of range of the string's memory
	vds_string_t out;

	if (str_len <= 0) 
	{ 
		char * write_location = (char *)vds_allocator_allocate_aligned(alloc, 1, 1);
		*write_location = '\0';

		out._str = write_location;
		out._len = 0;
	}
	else
	{
		int64_t str_len_with_nullterm = str_len + 1;

		char * write_location = (char *)vds_allocator_allocate_aligned(alloc, str_len_with_nullterm, 1);
		memcpy(write_location, string + low, str_len);
		write_location[str_len] = '\0';

		out._len = str_len;
		out._str = write_location;
	}

	return out;
}

static inline vds_string_t vstr(char const * string, vds_allocator_t * alloc)
{
	return vds_string_create(string, alloc);
}

static inline char const * vds_string_raw(vds_string_t str)
{
	return str._str;
}

static inline bool vds_string_is_empty(vds_string_t str)
{
	return (bool)(str._len);
}

static inline bool vds_string_equals(vds_string_t left, vds_string_t right)
{
	if (left._len != right._len) { return false; }

	return ( strcmp(left._str, right._str) == 0 );
}

static inline vds_string_t vds_string_append(vds_string_t left, vds_string_t right, vds_allocator_t * alloc)
{
	int64_t byte_len = left._len + right._len + 1;
	char * write_location = (char *)vds_allocator_allocate_aligned(alloc, byte_len, 1);
	memcpy(write_location, left._str, left._len);
	memcpy(write_location + left._len, right._str, right._len);
	write_location[byte_len] = '\0';

	vds_string_t out;
	out._str = write_location;
	out._len = left._len + right._len;

	return out;
}

//TODO(zshoals 03-21-2023):> Make this work with multi-byte characters, this will not split properly on say, some japanese glyph
static inline vds_string_t vds_string_get_first_split(vds_string_t base, vds_string_t splitter, vds_allocator_t * alloc)
{
	int64_t segment_len = 0;
	int64_t const splitter_len = splitter._len;
	int64_t const end_len = base._len;

	while (segment_len < end_len)
	{
		if ( (base._str[segment_len] == splitter._str[0]) && ((segment_len + splitter_len) < end_len) )
		{
			if (strncmp(&base._str[segment_len], splitter._str, splitter_len) == 0) { break; }
			++segment_len;
		}
		else
		{
			++segment_len;
		}
	}

	return vds_string_create_range(base._str, 0, segment_len, alloc);
}

static inline vds_string_t vds_string_split(vds_string_t base, vds_string_t splitter, vds_allocator_t * alloc)
{
	int64_t segment_len = 0;
	int64_t const splitter_len = splitter._len;
	int64_t const end_len = base._len;

	while (segment_len < end_len)
	{
		if ( (base._str[segment_len] == splitter._str[0]) && ((segment_len + splitter_len) < end_len) )
		{
			if (strncmp(&base._str[segment_len], splitter._str, splitter_len) == 0) 
			{ 
				segment_len += splitter_len; 
				break;
			}
			else
			{
				++segment_len;
			}
		}
		else
		{
			++segment_len;
		}
	}

	return vds_string_create_range(base._str, segment_len, end_len, alloc);
}