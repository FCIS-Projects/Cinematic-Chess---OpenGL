/*
 * projection.c
 *
 *  Created on: May 17, 2016
 *      Author: leo-0
 */

#include "projection.h"

static void init();
static mat4_t get_orthographic_projection_matrix();
static mat4_t get_prespective_projection_matrix( GLfloat zoom_aspect );
static void clean();

static GLuint __view_width = 0;
static GLuint __view_height = 0;

Namespace_Projection const Projection = {
	init,
	get_orthographic_projection_matrix,
	get_prespective_projection_matrix,
	clean,
};

void init( GLuint view_width, GLuint view_height )
{
	__view_width  = view_width;
	__view_height = view_height;
}

// TODO: need to be implemented
mat4_t get_orthographic_projection_matrix()
{

}

mat4_t get_prespective_projection_matrix( GLfloat zoom_aspect )
{
	mat4_t prespective = mat4_create(NULL);
	mat4_perspective( zoom_aspect, __view_width / __view_height,
										0.1f, 100.0f, prespective );
}

void clean()
{

}
