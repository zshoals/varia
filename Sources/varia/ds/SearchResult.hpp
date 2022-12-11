#pragma once

namespace vds
{

enum SearchResultStatus_e
{
	Missing,
	Found
};

template<typename T>
struct SearchResult
{
	T const * value;
	SearchResultStatus_e was_found;
};

template<typename T>
struct SearchResultMut
{
	T * value;
	SearchResultStatus_e was_found;
};


}