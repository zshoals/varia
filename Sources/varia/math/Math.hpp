#pragma once

constexpr double EPSILON = 0.00000001;
constexpr double PI = 3.145926545;
constexpr double TAU = PI * 2;

constexpr inline float vmath_lerp(float value, float start, float end)
{
	return start * (1 - value) + (end * value);
}

constexpr inline float vmath_inverse_lerp(float value, float start, float end)
{
	return (value - start) / (end - start);
}

constexpr inline float vmath_remap(float value, float s1, float e1, float s2, float e2)
{
	return vmath_inverse_lerp(vmath_lerp(value, s1, e1), s2, e2);
}

constexpr inline float vmath_rad2deg(float radians)
{
	return static_cast<float>(radians * (180.0 / PI));
}

constexpr inline float vmath_deg2rad(float degrees)
{
	return static_cast<float>(degrees * (PI / 180.0));
}

constexpr inline float vmath_minF(float a, float b)
{
	return (a < b) ? a : b;
}

constexpr inline float vmath_maxF(float a, float b)
{
	return (a > b) ? a : b;
}

constexpr inline int vmath_min(int a, int b)
{
	return (a < b) ? a : b;
}

constexpr inline int vmath_max(int a, int b)
{
	return (a > b) ? a : b;
}

constexpr inline size_t vmath_minST(size_t a, size_t b)
{
	return (a < b) ? a : b;
}

constexpr inline size_t vmath_maxST(size_t a, size_t b)
{
	return (a > b) ? a : b;
}

constexpr inline float vmath_clampF(float value, float lower, float upper)
{
	if (value < lower) value = lower;
	if (value > upper) value = upper;

	return value;
}

constexpr inline int vmath_clamp(int value, int lower, int upper)
{
	if (value < lower) value = lower;
	if (value > upper) value = upper;

	return value;
}

template <typename T>
constexpr inline T vmath_mod(T a, T b)
{
	return (a % b + b) % b;
}

template <typename T>
constexpr inline T vmath_ceil_next_nearest(T value, T target_multiple)
{
	bool round_up = (value % target_multiple == 0) ? false : true;
	return (((target_multiple - (value % target_multiple)) * round_up) + value);
}

constexpr static u8 vmath_pow2_to_bitshift_value(size_t pow2_value)
{
	u8 shift = 0;
	while (pow2_value > 1)
	{
		pow2_value /= 2;
		++shift;
	}

	return shift;
}

constexpr static size_t vmath_pow2_next_nearest(size_t value)
{
	--value;
	value |= (value >> 1);
	value |= (value >> 2);
	value |= (value >> 4);
	value |= (value >> 8);
	value |= (value >> 16);
	++value;

	return value;
}
