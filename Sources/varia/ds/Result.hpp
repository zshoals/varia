#pragma once

enum vds_result_status_e
{
	VDS_RESULT_STATUS_FAILURE_E,
	VDS_RESULT_STATUS_SUCCESS_E,
};

template<typename T>
struct vds_result_t
{
	T value;
	vds_result_status_e valid;
};