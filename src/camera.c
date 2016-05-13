/*
 * camera.c
 *
 *  Created on: May 1, 2016
 *      Author: leo-0
 */

#include "camera.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#ifndef PI
#define PI		3.14159265358979323846
#endif

#define TO_RADIAN(angle) (angle / 180.0f * PI )

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat ROLL       =  0.0f;
const GLfloat SPEED      =  5.0f;
const GLfloat SENSITIVTY =  0.5f;
const GLfloat ZOOM       =  45.0f;

// Camera position
vec3_t camera_position = NULL;
// The up direction. Camera’s y-axis
vec3_t camera_up_axis = NULL;
// The right direction. Camera’s x-axis
vec3_t camera_right_axis = NULL;
// The -ve of the look direction. Camera’s z-axis
vec3_t camera_front_axis = NULL;
// this used for saving the the intial value of camera_up
vec3_t worldUp = NULL;

// Eular Angles
GLfloat _yaw   = 0.0f;
GLfloat _pitch = 0.0f;
GLfloat _roll  = 0.0f;

// Camera options
GLfloat movement_speed 		 = 0.0f;
GLfloat movement_sensitivity = 0.0f;
GLfloat _zoom_aspect 		 = 0.0f;

// this is TRUE if in a testing mode
bool testing;

// TODO: add a function that handle mouse movement

static void init( vec3_t cam_pos, vec3_t cam_up_axis, vec3_t cam_right_axis,
		vec3_t cam_front_axis, GLfloat yaw, GLfloat pitch, GLfloat roll );
// 'testing' is used if we want to use default coordinates for the camera
static void init_test();
static void update_view_matrix();
static mat4_t get_view_matrix();
static void zoom( GLfloat yoffset );
static GLfloat get_zoom_aspect();

static void set_camera_coordinates( vec3_t cam_pos, vec3_t cam_up_axis,
		vec3_t cam_right_axis, vec3_t cam_front_axis );
static void set_camera_position( vec3_t cam_pos );
static void set_camera_up_axis( vec3_t cam_up_axis );
static void set_camera_right_axis( vec3_t cam_right_axis );
static void set_camera_front_axis( vec3_t cam_front_axis );

static void set_yaw_pitch_roll( GLfloat yaw, GLfloat pitch, GLfloat roll );
static void set_yaw( GLfloat angle );
static void set_pitch( GLfloat angle );
static void set_roll( GLfloat angle );
static void move_camera_without_delta( Camera_Movement direction );
static void move_camera( Camera_Movement direction, GLfloat delta_time );
//static void move_camera_inner( Camera_Movement direction, GLfloat velocity );
static void clean();

static void set_camera_moving_speed( GLfloat speed );
static void set_camera_senstivity( GLfloat speed );
static void set_camera_zoom_aspect( GLfloat speed );

Namespace_Camera const Camera = {
		init, init_test,
		get_view_matrix,
		zoom,
		set_camera_coordinates,
		update_view_matrix,
		set_camera_position,
		set_camera_up_axis,
		set_camera_right_axis,
		set_camera_front_axis,
		set_yaw_pitch_roll,
		set_yaw,
		set_pitch,
		set_roll,
		move_camera_without_delta,
		move_camera,
		set_camera_moving_speed,
		set_camera_senstivity,
		set_camera_zoom_aspect,
		clean,
		get_zoom_aspect };

void init( vec3_t cam_pos, vec3_t cam_up_axis, vec3_t cam_right_axis,
		vec3_t cam_front_axis, GLfloat yaw, GLfloat pitch, GLfloat roll )
{
	_yaw = yaw;
	_pitch = pitch;
	_roll = roll;

	movement_speed = SPEED;
	movement_sensitivity = SENSITIVTY;
	_zoom_aspect = ZOOM;

	camera_position = cam_pos;
	camera_up_axis = cam_up_axis;
	worldUp = camera_up_axis;
	camera_right_axis = cam_right_axis;
	camera_front_axis = cam_front_axis;

	testing = false;
}

void init_test()
{
	camera_position = vec3_create( NULL );
	camera_position[0]= 0.0f;
	camera_position[1]= 0.0f;
	camera_position[2]= 3.0f;

	camera_up_axis = vec3_create( NULL );
	camera_up_axis[0] = 0.0f;
	camera_up_axis[1] = 1.0f;
	camera_up_axis[2] = 0.0f;

	worldUp = camera_up_axis;

	camera_right_axis = vec3_create( NULL );

	camera_front_axis = vec3_create( NULL );

	_yaw = YAW;
	_pitch = PITCH;
	_roll = ROLL;

	movement_speed = SPEED;
	movement_sensitivity = SENSITIVTY;
	_zoom_aspect = ZOOM;

	update_view_matrix();

	testing = true;
}

void set_camera_coordinates( vec3_t cam_pos, vec3_t cam_up_axis, vec3_t cam_right_axis,
								vec3_t cam_front_axis )
{
	// clean old ones
	if(camera_position) free(camera_position);
	if(camera_up_axis) free(camera_up_axis);
	if(camera_right_axis) free(camera_right_axis);
	if(camera_front_axis) free(camera_front_axis);

	camera_position = cam_pos;
	camera_up_axis = cam_up_axis;
	worldUp = camera_up_axis;
	camera_right_axis = cam_right_axis;
	camera_front_axis = cam_front_axis;
}

void set_camera_position( vec3_t cam_pos )
{
	if(camera_position) free(camera_position);

	camera_position = cam_pos;

	update_view_matrix();
}

void set_camera_up_axis( vec3_t cam_up_axis )
{
	if(camera_up_axis) free(camera_up_axis);

	camera_up_axis = cam_up_axis;
	worldUp = camera_up_axis;

	update_view_matrix();
}

void set_camera_right_axis( vec3_t cam_right_axis )
{
	if(camera_right_axis) free(camera_right_axis);

	camera_right_axis = cam_right_axis;

	update_view_matrix();
}

void set_camera_front_axis( vec3_t cam_front_axis )
{
	if(camera_front_axis) free(camera_front_axis);

	camera_front_axis = cam_front_axis;

	update_view_matrix();
}

mat4_t get_view_matrix()
{
	// make sure that init() or init_test() has been called
	assert( camera_position != NULL );

	vec3_t temp;
	temp = vec3_create(NULL);

	mat4_t view_matrix;
	view_matrix = mat4_create(NULL);

	vec3_add( camera_position, camera_front_axis, temp );
	mat4_lookAt( camera_position, temp, camera_up_axis, view_matrix );

	free(temp);

	return view_matrix;
}

void update_view_matrix()
{
	// make sure that init() or init_test() has been called
	assert( camera_front_axis != NULL );

	/* It's based on Eular's therom */
	vec3_t target = NULL;
	target = vec3_create( NULL );
	target[0] = cosf( TO_RADIAN(_yaw) ) * cosf( TO_RADIAN(_pitch) );
	target[1] = sinf( TO_RADIAN(_pitch) );
	target[2] = sinf( TO_RADIAN(_yaw) ) * cosf( TO_RADIAN(_pitch) );

	vec3_normalize( target, camera_front_axis );

	// Camera_Right
	vec3_cross( camera_front_axis, worldUp, camera_right_axis );
	vec3_normalize(camera_right_axis, camera_right_axis);
	// Camera_Up
	vec3_cross( camera_right_axis, camera_front_axis, camera_up_axis );
	vec3_normalize(camera_up_axis, camera_up_axis);

	free(target);
}

void zoom( GLfloat yoffset )
{
	if( _zoom_aspect > 180.0f && yoffset == -1.0f )
		return;

	else if( _zoom_aspect < 0.0 && yoffset == 1.0f )
		return;

	_zoom_aspect -= yoffset;
}

GLfloat get_zoom_aspect()
{
	return _zoom_aspect;
}

void set_yaw_pitch_roll( GLfloat yaw, GLfloat pitch, GLfloat roll )
{
	_yaw = yaw;
	_pitch = pitch;
	_roll = roll;

	update_view_matrix();
}

void set_yaw( GLfloat angle )
{
	_yaw = angle;
	update_view_matrix();
}

void set_pitch( GLfloat angle )
{
	_pitch = angle;
	update_view_matrix();
}

void set_roll( GLfloat angle )
{
	_roll = angle;
	update_view_matrix();
}

void move_camera_without_delta( Camera_Movement direction )
{
	// make sure that init() or init_test() has been called
	assert( camera_position );

	switch( direction )
	{
		case FORWARD:
			vec3_add(camera_position, camera_front_axis, camera_position);
			break;
		case BACKWARD:
			vec3_subtract(camera_position, camera_front_axis, camera_position);
			break;
		case LEFT:
			vec3_add(camera_position, camera_right_axis, camera_position);
			break;
		case RIGHT:
			vec3_subtract(camera_position, camera_right_axis, camera_position);
			break;
	};
}

void move_camera( Camera_Movement direction, GLfloat delta_time )
{
	// make sure that init() or init_test() has been called
	assert( camera_position != NULL );

	GLfloat speed = movement_speed * delta_time;
	vec3_t temp = vec3_create(NULL);

	switch( direction )
	{
		case FORWARD:
			vec3_scale(camera_front_axis, speed, temp);
			vec3_add(camera_position, temp, camera_position);
			break;
		case BACKWARD:
			vec3_scale(camera_front_axis, speed, temp);
			vec3_subtract(camera_position, temp, camera_position);
			break;
		case LEFT:
			vec3_scale(camera_right_axis, speed, temp);
			vec3_subtract(camera_position, temp, camera_position);
			break;
		case RIGHT:
			vec3_scale(camera_right_axis, speed, temp);
			vec3_add(camera_position, temp, camera_position);
			break;
	};

	free(temp);
}

void set_camera_moving_speed( GLfloat speed )
{
	movement_speed = speed;
}

void set_camera_senstivity( GLfloat senstivity )
{
	movement_sensitivity = senstivity;
}

void set_camera_zoom_aspect( GLfloat zoom_aspect )
{
	_zoom_aspect = zoom_aspect;
}

void clean()
{
	if( testing )
	{
		free(camera_position);
		free(camera_up_axis);
		free(camera_right_axis);
		free(camera_front_axis);
	}

	camera_position = NULL;
	camera_up_axis = NULL;
	worldUp = NULL;
	camera_right_axis = NULL;
	camera_front_axis = NULL;
}
