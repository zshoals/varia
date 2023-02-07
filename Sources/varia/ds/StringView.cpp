#include "StringView.hpp"

#include "varia/logging.hpp"
#include "varia/math/MinMax.hpp"
#include <string.h>

vds_strview_t vds_strview_create_with_length(char const * null_term_string, size_t length)
{
	vds_strview_t sv;
	sv.string = null_term_string;
	sv._length = length;

	return sv;
}

vds_strview_t vds_strview_create(char const * null_term_string)
{
	vds_strview_t sv;
	sv.string = null_term_string;
	sv._length = strnlen(null_term_string, VDS_STRVIEW_CONSTANTS_MAX_LENGTH);

	return sv;
}

bool vds_strview_matches(vds_strview_t a, vds_strview_t b)
{
	if (a._length != b._length) return false;

	if (strncmp(a.string, b.string, a._length) == 0) return true;
	return false;
}

vds_strview_t vds_strview_strip_leading_glyph(vds_strview_t stringview, char const * glyph)
{
	if (*glyph == '\0') return stringview;

	for_range_var(i, stringview._length)
	{
		if (vds_strview_raw(stringview)[i] != glyph[0])
		{
			size_t const new_len = stringview._length - i;
			return vds_strview_create_with_length(&vds_strview_raw(stringview)[i], new_len);
		}
	}

	return vds_strview_create_with_length("", 0);
}

vds_strview_t vds_strview_strip_trailing_glyph(vds_strview_t stringview, char const * glyph)
{
	if (*glyph == '\0') return stringview;

	for_reverse_range_var(i, stringview._length)
	{
		if (vds_strview_raw(stringview)[i] != glyph[0])
		{
			size_t const new_len = i + 1;
			return vds_strview_create_with_length(&vds_strview_raw(stringview)[0], new_len);
		}
	}

	return vds_strview_create_with_length("", 0);
}

char const * vds_strview_raw(vds_strview_t stringview)
{
	return stringview.string;
}

vds_result_t<vds_strview_t> vds_strview_find_first_occurrence(vds_strview_t haystack, vds_strview_t needle)
{
	vds_result_t<vds_strview_t> res = {};
	{
		res.valid = VDS_RESULT_STATUS_FAILURE_E;
		res.value = vds_strview_create_with_length("", 0);
	}

	if (needle._length > haystack._length) return res;

	char const * searched_string = strstr(haystack.string, needle.string);
	if (!searched_string) return res;

	res.valid = VDS_RESULT_STATUS_SUCCESS_E;
	res.value = vds_strview_create_with_length(searched_string, needle._length);

	return res;
}

void vds_strview_print(vds_strview_t stringview)
{
	//Nothing to write
	if (stringview._length == 0) return;

	char buffer[VDS_STRVIEW_CONSTANTS_MAX_LENGTH] = {};
	size_t write_len = vmath_minST(stringview._length, VDS_STRVIEW_CONSTANTS_MAX_LENGTH - 1); // Save room for null term
	memcpy(&buffer, stringview.string, write_len);
	buffer[write_len] = '\0';

	char const * p_buffer = &buffer[0];
	kinc_log(KINC_LOG_LEVEL_INFO, p_buffer);
}
