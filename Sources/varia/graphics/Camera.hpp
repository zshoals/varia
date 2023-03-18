#pragma once

#include "varia/lib/cglm/types-struct.h"

struct varia_environment_t;

struct varia_graphics_camera_t
{
	vec3s position;
	float rotation;
	float zoom_factor;
};

void varia_graphics_camera_initialize(varia_graphics_camera_t * cam);

mat4s varia_graphics_camera_as_ortho_full(varia_graphics_camera_t * cam, float left, float right, float bottom, float top, float near, float far);
mat4s varia_graphics_camera_as_ortho(varia_graphics_camera_t * cam, varia_environment_t * env);

void varia_graphics_camera_add_move(varia_graphics_camera_t * cam, vec3s shift);
void varia_graphics_camera_add_rotation(varia_graphics_camera_t * cam, float radians);
void varia_graphics_camera_add_zoom(varia_graphics_camera_t * cam, float zoom_factor);

void varia_graphics_camera_set_move(varia_graphics_camera_t * cam, vec3s shift);
void varia_graphics_camera_set_rotation(varia_graphics_camera_t * cam, float radians);
void varia_graphics_camera_set_zoom(varia_graphics_camera_t * cam, float zoom_factor);
