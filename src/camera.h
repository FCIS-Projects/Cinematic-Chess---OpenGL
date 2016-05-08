/*
 * camera.h
 *
 *  Created on: May 1, 2016
 *      Author: leo-0
 */

#include "gl-matrix.h"
#include <GL/glew.h>
#include "bool.h"

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

typedef enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} Camera_Movement;

typedef struct
{
	void (*init)( vec3_t cam_pos, vec3_t cam_up_axis, vec3_t cam_right_axis,
			vec3_t cam_front_axis, GLfloat yaw, GLfloat pitch, GLfloat roll );
	void(*init_test)();
	mat4_t (*get_view_matrix)();
	void (*zoom)( GLfloat yoffset );
	void (*set_camera_coordinates)( vec3_t cam_pos, vec3_t cam_up_axis,
			vec3_t cam_right_axis, vec3_t cam_front_axis );
	void (*update_view_matrix)();
	void (*set_camera_position)( vec3_t cam_pos );
	void (*set_camera_right_axis)( vec3_t cam_right_axis );
	void (*set_camera_front_axis)( vec3_t cam_front_axis );
	void (*set_camera_up_axis)( vec3_t cam_up_axis );
	void (*set_yaw_pitch_roll)( GLfloat yaw, GLfloat pitch, GLfloat roll );
	void (*set_yaw)( GLfloat angle );
	void (*set_pitch)( GLfloat angle );
	void (*set_roll)( GLfloat angle );
	void (*move_camera_without_delta)( Camera_Movement direction );
	void (*move_camera)( Camera_Movement direction, GLfloat delta_time );
	void (*set_camera_moving_speed)( GLfloat speed );
	void (*set_camera_senstivity)( GLfloat senstivity );
	void (*set_camera_zoom_aspect)( GLfloat zoom_aspect );
	void (*clean)();
	GLfloat (*get_zoom_aspect)();
} Namespace_Camera;

extern Namespace_Camera const Camera;

#endif /* SRC_CAMERA_H_ */
