#pragma once

//TODO(zshoals 01-31-2023):> Make this an enum class
enum vds_search_result_status_e
{
	VDS_SEARCH_RESULT_STATUS_MISSING_E,
	VDS_SEARCH_RESULT_STATUS_FOUND_E
};

template<typename T>
struct vds_search_result_t
{
	T const * value;
	vds_search_result_status_e was_found;
};

template<typename T>
struct vds_search_result_mut_t
{
	T * value;
	vds_search_result_status_e was_found;
};