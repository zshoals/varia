// #pragma once

// #include "varia/math/MathTypes.hpp"
// #include "varia/math/Math.hpp"

// constexpr inline vec2 vec2_normalize(vec2 v)
// {
// 	float len = sqrtf(vmath_pow2(v.x) + vmath_pow2(v.y));

// 	vec2 v2 = {};
// 	v2.x = v.x / len;
// 	v2.y = v.y / len;

// 	return v2;
// }

// constexpr inline float vmath_dot(vec2 a, vec2 b)
// {
// 	return (a.x * b.x) + (a.y * b.y);
// }

// constexpr inline vec2 vec2_add(vec2 a, vec2 b)
// {
// 	return {a.x + b.x, a.y + b.y};
// }

// constexpr inline vec2 vec2_sub(vec2 a, vec2 b)
// {
// 	return {a.x - b.x, a.y - b.y};
// }

// constexpr inline vec2 vec2_mul(vec2 a, vec2 b)
// {
// 	return {a.x * b.x, a.y * b.y};
// }

// constexpr inline vec2 vec2_div(vec2 a, vec2 b)
// {
// 	return {a.x / b.x, a.y / b.y};
// }

// constexpr inline vec2 vec2_flip_horizontal(vec2 v)
// {
// 	return {-v.x, v.y};
// }

// constexpr inline vec2 vec2_flip_vertical(vec2 v)
// {
// 	return {v.x, -v.y};
// }

// constexpr inline float vec2_squared_length(vec2 v)
// {
// 	return vmath_pow2(v.x) + vmath_pow2(v.y);
// }

// constexpr inline float vec2_length(vec2 v)
// {
// 	return sqrtf(vec2_squared_length(v));
// }