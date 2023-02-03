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
};

vds_strview_t vds_strview_create(char const * null_term_string);
vds_strview_t vds_strview_create_with_length(char const * null_term_string, size_t length);
bool vds_strview_matches(vds_strview_t a, vds_strview_t b);
char const * vds_strview_raw(vds_strview_t stringview);
vds_result_t<vds_strview_t> vds_strview_find_first_occurrence(vds_strview_t haystack, vds_strview_t needle);
void vds_strview_print(vds_strview_t stringview);





template<int MaxSize>
size_t vds_strview_sequence_length(vds_strview_sequence_t<MaxSize> * sequence)
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
void vds_strview_sequence_buffer_size_requirement(vds_strview_sequence_t<MaxSize> * sequence)
{
	size_t bytes_usage = 0;

	for(vds_strview_t const & strview : *sequence)
	{
		bytes_usage += strview._length;
		bytes_usage += 1; //Account for Null terminator
	}
}

//Note(zshoals 02-03-2023):> Only handles a 1 character sized separator at present
template<int MaxSize>
vds_strview_sequence_t<MaxSize> vds_strview_split_by(vds_strview_t stringview, char const * separator)
{
	vds_strview_sequence_t<MaxSize> svs = {};
	if (stringview._length == 0 || *separator == '\0') return svs;

	size_t remaining_chars = stringview._length;
	size_t current_char_idx = 0;
	char const * base_string_addr = vds_strview_raw(stringview);

	//Strip leading characters equivalent to the separator
	{
		while(base_string_addr[current_char_idx] == *separator && remaining_chars != 0)
		{
			++current_char_idx;
			--remaining_chars;
		}
	}

	//Actually start splitting stringviews
	size_t sequence_length = 0;
	{

		while(remaining_chars - 1 != 0)
		{
			//We've found a character
			if (base_string_addr[sequence_length + current_char_idx] != *separator)
			{
				++sequence_length;
			}

			//We've encountered a repeated separator (IE, 2 spaces in a row)
			else if (sequence_length == 0) 
			{
				++current_char_idx;
			}

			//We've encountered a single separator that is not duplicated
			else
			{
				vds_strview_t sv;
				sv.string = &base_string_addr[current_char_idx];
				sv._length = sequence_length;

				current_char_idx += sequence_length;
				++current_char_idx;
				sequence_length = 0;

				vds_array_try_push(&svs.strviews, sv);
			}

			--remaining_chars;
		}
	}

	//Special case: We've depleted characters remaining, but there is still a character sequence
	//that hasn't been processed yet
	if (sequence_length > 0)
	{
		vds_strview_t sv;
		sv.string = &base_string_addr[current_char_idx];
		sv._length = sequence_length;

		vds_array_try_push(&svs.strviews, sv);
	}

	return svs;
}


template<int MaxSize>
vds_strview_sequence_t<MaxSize> vds_strview_internal_split_by_line(vds_strview_t stringview)
{
	vds_strview_sequence_t<MaxSize> svs = {};
	vds_array_initialize(&svs.strviews);

	if (stringview._length == 0) return svs;

	//This-is-a-string
	//---This--is--a-string-

	char const * base_string_addr = stringview.string;
	size_t string_end_idx = stringview._length;

	size_t sequence_begin_idx = 0;
	size_t sequence_length = 0;

	//Strip leading separators
	for(size_t i = 0; i < stringview._length; ++i)
	{
		if (base_string_addr[i] == '\r')
		{
			if (i + 1 < stringview._length)
			{
				if (base_string_addr[i + 1] == '\n')
				{
					sequence_begin_idx += 2;
				}
			}
		}
		else if (base_string_addr[i] == '\n')
		{
			++sequence_begin_idx;
		}
		//All leading newlines are culled
		else
		{
			break;
		}
	}

	//Strip tailing separators
	for(size_t i = string_end_idx; i > sequence_begin_idx; --i)
	{

	}

	while(sequence_begin_idx < string_end_idx)
	{
		if 
		(
			(base_string_addr[sequence_begin_idx + sequence_length] == *separator)
			||
			(sequence_begin_idx + sequence_length >= string_end_idx)
		)
		{
			vds_strview_t view;
			view.string = &base_string_addr[sequence_begin_idx];
			view._length = sequence_length;

			vds_array_try_push(&svs.strviews, view);

			sequence_begin_idx += sequence_length;
			sequence_length = 0;

			//Strip subsequent separators (possibly duplicates)
			for(size_t i = sequence_begin_idx; i < stringview._length; ++i)
			{
				if (base_string_addr[i] != *separator) break;
				++sequence_begin_idx;
			}
		}
		//The character is part of a valid string sequence
		else if (base_string_addr[sequence_begin_idx + sequence_length] != *separator)
		{
			++sequence_length;
		}
	}

	return svs;

}

template<int MaxSize>
vds_strview_sequence_t<MaxSize> vds_strview_split_by_v2(vds_strview_t stringview, char const * separator)
{
	//Special case new lines, as they may be slashN or slashR slashN
	// if (*separator == '\n')
	// {
	// 	return vds_strview_internal_split_by_line(stringview);
	// }

	vds_strview_sequence_t<MaxSize> svs = {};
	vds_array_initialize(&svs.strviews);

	if (stringview._length == 0) return svs;

	//This-is-a-string
	//---This--is--a-string-

	char const * base_string_addr = stringview.string;
	size_t string_end_idx = stringview._length;

	size_t sequence_begin_idx = 0;
	size_t sequence_length = 0;

	while(sequence_begin_idx + sequence_length - 1 < string_end_idx)
	{
		if 
		(
			base_string_addr[sequence_begin_idx + sequence_length] == *separator
			&&
			sequence_length > 0
		)
		{
			vds_strview_t view;
			view.string = &base_string_addr[sequence_begin_idx];
			view._length = sequence_length;

			vds_array_try_push(&svs.strviews, view);

			sequence_begin_idx += sequence_length;
			sequence_length = 0;

			//Strip subsequent separators (possibly duplicates)
			for(size_t i = sequence_begin_idx; i < stringview._length; ++i)
			{
				if (base_string_addr[i] != *separator) break;
				++sequence_begin_idx;
			}
		}
		// Repeating/lead/trailing seperator
		else if
		(
			base_string_addr[sequence_begin_idx + sequence_length] == *separator
			&&
			sequence_length == 0
		)
		{
			++sequence_begin_idx;
		}
		//The character is part of a valid string sequence
		else if (base_string_addr[sequence_begin_idx + sequence_length] != *separator)
		{
			++sequence_length;
		}
	}

	if (sequence_length > 0)
	{
			vds_strview_t view;
			view.string = &base_string_addr[sequence_begin_idx];
			view._length = sequence_length;

			vds_array_try_push(&svs.strviews, view);
	}

	return svs;

}


//Note(zshoals 02-02-2023):> Useless? We still need to figure out a way to actually track each individual string
//having it all in a buffer doesn't actually make this usable

// //TODO(zshoals 02-02-2023):> Maybe we should have some sort of "memory region" struct that contains
// //a buffer of bytes and a length instead of just YOLO'ing it.
// template <int MaxSize>
// void vds_strview_write_sequence_to_buffer(vds_strview_sequence_t<MaxSize> * sequence, char * buffer, size_t buffer_len)
// {
// 	char * buffer_write_head = buffer;

// 	for(vds_strview_t const & sv : *sequence)
// 	{
// 		//We have no reason to write out an empty string
// 		if (sv._length == 0) continue;

// 		size_t sv_read_idx = 0;
// 		while (buffer_len > 0 && sv_read_idx < sv._length)
// 		{
// 			*write_head = sv.string[sv_read_idx];
// 			++write_head;
// 			++sv_read_idx;
// 		}

// 		*write_head = '\0';
// 		++write_head;
// 	}
// }

// //OLD
// //OLD
// template<int MaxSize>
// vds_strview_sequence_t<MaxSize> vds_strview_split_by(vds_strview_t stringview, char const * separator)
// {
// 	vds_strview_sequence_t<MaxSize> svs = {};
// 	if (stringview._length == 0 || *separator == '\0') return svs;

// 	char const * search_head = vds_strview_raw(stringview);
// 	size_t remaining_chars = stringview._length;

// 	//Ignore leading separators
// 	while(*search_head == *separator && remaining_chars != 0)
// 	{
// 		++search_head;
// 		--remaining_chars;
// 	}


// 	//Actually start splitting stringviews
// 	char const * first_split = search_head;
// 	size_t sequence_length = 0;

// 	while(*search_head != '\0' && remaining_chars != 0)
// 	{
// 		if (*search_head == *separator)
// 		{
// 			vds_strview_t sv;
// 			sv.string = first_split;
// 			sv._length = sequence_length;

// 			if (vds_array_length(&svs.strviews) >= MaxSize) break;
// 			vds_array_push(&svs.strviews, sv);

// 			//Ignore repeat separators
// 			while(*search_head == *separator && remaining_chars != 0)
// 			{
// 				++search_head;
// 				--remaining_chars;
// 			}

// 			//Reset the split searches
// 			first_split = search_head;
// 			sequence_length = 0;
// 		}

// 		++search_head;
// 		--remaining_chars;
// 		++sequence_length;
// 	}

// 	//Final split needs special management
// 	if (sequence_length > 1)
// 	{
// 		if (vds_array_length(&svs.strviews) < MaxSize)
// 		{
// 			vds_strview_t sv;
// 			sv.string = first_split;
// 			sv._length = sequence_length;

// 			vds_array_push(&svs.strviews, sv);
// 		}
// 	}

// 	return svs;
// }
