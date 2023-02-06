#pragma once

#include "varia/vcommon.hpp"

#include "Result.hpp"
#include "StaticArray.hpp"

#define VDS_STRVIEW_CONSTANTS_MAX_LENGTH 4096

struct vds_strview_t
{
	char const * string;
	size_t _length;
};

template<int MaxSize>
struct vds_strview_sequence_t
{
	vds_array_t<vds_strview_t, MaxSize> strviews;
	auto begin(void) { return strviews.begin(); }
	auto end(void) { return strviews.end(); }
	auto cbegin(void) const { return begin(); }
	auto cend(void) const { return end(); }
};

vds_strview_t vds_strview_create(char const * null_term_string);
vds_strview_t vds_strview_create_with_length(char const * null_term_string, size_t length);
bool vds_strview_matches(vds_strview_t a, vds_strview_t b);
char const * vds_strview_raw(vds_strview_t stringview);
vds_result_t<vds_strview_t> vds_strview_find_first_occurrence(vds_strview_t haystack, vds_strview_t needle);
void vds_strview_print(vds_strview_t stringview);
vds_strview_t vds_strview_strip_leading_glyph(vds_strview_t stringview, char const * glyph);
vds_strview_t vds_strview_strip_trailing_glyph(vds_strview_t stringview, char const * glyph);





template<int MaxSize>
void vds_strview_sequence_initialize(vds_strview_sequence_t<MaxSize> * self)
{
	vds_array_initialize(&self->strviews);
}

template<int MaxSize>
size_t vds_strview_sequence_length(vds_strview_sequence_t<MaxSize> const * sequence)
{
	return vds_array_length(&sequence->strviews);
}

template<int MaxSize>
void vds_strview_sequence_print_all(vds_strview_sequence_t<MaxSize> * sequence)
{
	for (vds_strview_t const & str : *sequence)
	{
		vds_strview_print(str);
	}
}

template <int MaxSize>
void vds_strview_sequence_push(vds_strview_sequence_t<MaxSize> * self, vds_strview_t stringview)
{
	vds_array_push(&self->strviews, stringview);
}

template <int MaxSize>
bool vds_strview_sequence_try_push(vds_strview_sequence_t<MaxSize> * self, vds_strview_t stringview)
{
	return vds_array_try_push(&self->strviews, stringview);
}

template <int MaxSize>
vds_strview_t vds_strview_sequence_pop(vds_strview_sequence_t<MaxSize> * self, vds_strview_t stringview)
{
	return vds_array_pop(&self->strviews);
}

template <int MaxSize>
vds_strview_sequence_t<MaxSize> vds_strview_internal_split_newline(vds_strview_t string)
{
	vds_strview_sequence_t<MaxSize> svs;
	vds_strview_sequence_initialize(&svs);

	char const * raw_string = vds_strview_raw(string);
	size_t sequence_length = 0;
	size_t counter = 0;
	size_t const string_end_index = string._length;

	while (counter != string_end_index)
	{
		while ((raw_string[counter] == '\n' || raw_string[counter] == '\r') && counter != string_end_index)
		{
			if (sequence_length > 0)
			{
				size_t last_char_index = counter;
				size_t first_char_index = last_char_index - sequence_length;
				vds_strview_t view = vds_strview_create_with_length(&raw_string[first_char_index], sequence_length);
				vds_strview_sequence_try_push(&svs, view);

				sequence_length = 0;
			}

			++counter;
		}

		while ((raw_string[counter] != '\n' && raw_string[counter] != '\r') && counter != string_end_index)
		{
			++sequence_length;
			++counter;
		}
	}

	if (sequence_length > 0)
	{
		size_t last_char_index = counter;
		size_t first_char_index = last_char_index - sequence_length;
		vds_strview_t view = vds_strview_create_with_length(&raw_string[first_char_index], sequence_length);
		vds_strview_sequence_try_push(&svs, view);
	}
	

	return svs;
}

template <int MaxSize>
vds_strview_sequence_t<MaxSize> vds_strview_split(vds_strview_t string, char const * separator)
{

	vds_strview_sequence_t<MaxSize> svs;
	vds_strview_sequence_initialize(&svs);

	if (string._length == 0) return svs;



	//Special case newline splitting as it may be either "just" a slashN or both slashR and slashN
	if (*separator == '\n' || *separator == '\r') 
	{
		return vds_strview_internal_split_newline<MaxSize>(string);
	}



	//Generic case with a single character to split on
	char const * raw_string = vds_strview_raw(string);
	size_t sequence_length = 0;
	size_t counter = 0;
	size_t const string_end_index = string._length;

	while (counter != string_end_index)
	{
		while (raw_string[counter] == *separator && counter != string_end_index)
		{
			if (sequence_length > 0)
			{
				size_t last_char_index = counter;
				size_t first_char_index = last_char_index - sequence_length;
				vds_strview_t view = vds_strview_create_with_length(&raw_string[first_char_index], sequence_length);
				vds_strview_sequence_try_push(&svs, view);

				sequence_length = 0;
			}

			++counter;
		}

		while (raw_string[counter] != *separator && counter != string_end_index)
		{
			++sequence_length;
			++counter;
		}
	}

	//Handle the last remaining string sequence that wasn't processed in the previous loop
	if (sequence_length > 0)
	{
		size_t last_char_index = counter;
		size_t first_char_index = last_char_index - sequence_length;
		vds_strview_t view = vds_strview_create_with_length(&raw_string[first_char_index], sequence_length);
		vds_strview_sequence_try_push(&svs, view);
	}
	

	return svs;
}