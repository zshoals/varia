#pragma once

#include "MathTypes.hpp"

#include "varia/Vcommon.hpp"
#include <string.h>


mat4 mat4_identity(void) 
{
	mat4 out;

	memset(&out, 0, sizeof(mat4));
	out.m[0] = 1.0f;
	out.m[5] = 1.0f;
	out.m[10] = 1.0f;
	out.m[15] = 1.0f;

	return out;
}

mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far)
{
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(far + near) / (far - near);

	float twoRL = (2.0f / (right - left));
	float twoTB = (2.0f / (top - bottom));
	float twoFN = (-2.0f / (far - near));

	mat4 out = mat4_identity();
	out.m[0] = twoRL;
	out.m[3] = tx;
	out.m[5] = twoTB;
	out.m[7] = ty;
	out.m[10] = twoFN;
	out.m[11] = tz;
	out.m[15] = 1.0f;
}
// public static function orthogonalProjection(left: Float, right: Float, bottom: Float, top: Float, zn: Float, zf: Float): Matrix4 {
// 		var tx: Float = -(right + left) / (right - left);
// 		var ty: Float = -(top + bottom) / (top - bottom);
// 		var tz: Float = -(zf + zn) / (zf - zn);
// 		return new Matrix4(2 / (right - left), 0, 0, tx, 0, 2 / (top - bottom), 0, ty, 0, 0, -2 / (zf - zn), tz, 0, 0, 0, 1);