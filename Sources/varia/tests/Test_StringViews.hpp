// #pragma once

// #include "dread/Dread.hpp"
// #include "varia/ds/StringView.hpp"

// void test_strview_all_tests(void)
// {

// //TESTS BEGIN HERE




// 	DTEST("Strip Trailing Glyphs From Stringview",
// 	[](void) -> dread_result_t
// 	{
// 		vds_strview_t sv = vds_strview_create("Hello     ");
// 		sv = vds_strview_strip_trailing_glyph(sv, " ");
		
// 		if (vds_strview_matches(sv, vds_strview_create("Hello")))
// 		{
// 			return DREAD_SUCCESS;
// 		}
// 		return DREAD_FAILURE;
// 	});

// 	DTEST("Strip Leading Glyphs From Stringview", 
// 	[](void) -> dread_result_t
// 	{
// 		vds_strview_t sv = vds_strview_create("---Good");
// 		sv = vds_strview_strip_leading_glyph(sv, "-");

// 		if (vds_strview_matches(sv, vds_strview_create("Good")))
// 		{
// 			return DREAD_SUCCESS;
// 		}

// 		return DREAD_FAILURE;
// 	});

// 	DTEST("Empty Stringview Matches Empty Stringview",
// 	[](void) -> dread_result_t
// 	{
// 		vds_strview_t a = vds_strview_create("");
// 		vds_strview_t b = vds_strview_create("");

// 		if (vds_strview_matches(a, b))
// 		{
// 			return DREAD_SUCCESS;
// 		}

// 		return DREAD_FAILURE;
// 	});

// 	DTEST("Empty Stringview Does Not Match Stringview With Characters",
// 	[](void) -> dread_result_t
// 	{
// 		vds_strview_t a = vds_strview_create("");
// 		vds_strview_t b = vds_strview_create("Yo, what's going on over here Ralph?");

// 		//DOES NOT MATCH
// 		if (!vds_strview_matches(a, b))
// 		{
// 			return DREAD_SUCCESS;
// 		}

// 		return DREAD_FAILURE;
// 	});




// } //TESTS END HERE
