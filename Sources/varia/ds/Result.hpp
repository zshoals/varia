#pragma once

namespace vds
{

enum ResultStatus_e
{
	Failure,
	Success
};

template<typename T>
struct Result
{
	T value;
	ResultStatus_e valid;
};

}