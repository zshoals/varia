#pragma once


constexpr VARIA_INLINE float vmath_minF(float a, float b)
{
	return (a < b) ? a : b;
}

constexpr VARIA_INLINE float vmath_maxF(float a, float b)
{
	return (a > b) ? a : b;
}

constexpr VARIA_INLINE int vmath_min(int a, int b)
{
	return (a < b) ? a : b;
}

constexpr VARIA_INLINE int vmath_max(int a, int b)
{
	return (a > b) ? a : b;
}

constexpr VARIA_INLINE size_t vmath_minST(size_t a, size_t b)
{
	return (a < b) ? a : b;
}

constexpr VARIA_INLINE size_t vmath_maxST(size_t a, size_t b)
{
	return (a > b) ? a : b;
}
