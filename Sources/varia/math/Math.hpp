#pragma once

namespace Varia
{
namespace Math
{

template <typename T>
constexpr inline T mod(T a, T b)
{
	return (a % b + b) % b;
}

template <typename T>
constexpr inline T ceil_next_nearest(T value, T target_multiple)
{
	bool round_up = (value % target_multiple == 0) ? false : true;
	return (((target_multiple - (value % target_multiple)) * round_up) + value);
}

}
}