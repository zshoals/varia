#include "varia/graphics/Camera.hpp"

#include "varia/math/Math.hpp"

#include "varia/lib/cglm/struct/cam.h"
#include "varia/lib/cglm/struct/affine.h"
#include "varia/lib/cglm/struct/euler.h"
#include "varia/lib/cglm/struct/mat4.h"
#include "varia/lib/cglm/struct/vec3.h"


void varia_graphics_camera_initialize(varia_graphics_camera_t * cam)
{
	vec3s p;
	p.raw[0] = 0.0f;
	p.raw[1] = 0.0f;
	//Note(zshoals 03-17-2023):> Z axis is negated on orthographic creation
	p.raw[2] = 5.0f;

	cam->position = p;
	cam->rotation = 0.0f;
	cam->zoom_factor = 1.0f;
}

mat4s varia_graphics_camera_as_ortho
(
	varia_graphics_camera_t * cam,
	float left,
	float right,
	float bottom,
	float top,
	float near,
	float far
)
{
	vec3s rot_axis = {0.0f, 0.0f, -1.0f};
	mat4s cam_rot = glms_rotate_make(-cam->rotation, rot_axis);

	vec3s scale_uni = {cam->zoom_factor, cam->zoom_factor, 1.0f};
	mat4s cam_scale = glms_scale_make(scale_uni);

	mat4s cam_trans = glms_translate_make(glms_vec3_negate(cam->position));
	mat4s ortho = glms_ortho(left, right, bottom, top, near, far);

	mat4s out = glms_mat4_mul(cam_rot, cam_scale);
	out = glms_mat4_mul(cam_trans, out);
	out = glms_mat4_mul(ortho, out);

	return out;
}

void varia_graphics_camera_add_move(varia_graphics_camera_t * cam, vec2s shift)
{
	vec3s shift3 = {shift.raw[0], shift.raw[1], 0.0f};

	cam->position = glms_vec3_add(cam->position, shift3);
}

void varia_graphics_camera_add_rotation(varia_graphics_camera_t * cam, float radians)
{
	cam->rotation += radians;
	cam->rotation = vmath_wrap_radians(cam->rotation);
}

void varia_graphics_camera_add_zoom(varia_graphics_camera_t * cam, float zoom_factor)
{
	cam->zoom_factor += zoom_factor;
}

void varia_graphics_camera_set_move(varia_graphics_camera_t * cam, vec3s shift)
{
	cam->position = shift;
}

void varia_graphics_camera_set_rotation(varia_graphics_camera_t * cam, float radians)
{
	cam->rotation = radians;
	cam->rotation = vmath_wrap_radians(cam->rotation);
}

void varia_graphics_camera_set_zoom(varia_graphics_camera_t * cam, float zoom_factor)
{
	cam->zoom_factor = zoom_factor; 
}

