/*
 * projection.h
 *
 *  Created on: May 17, 2016
 *      Author: leo-0
 */

#ifndef PROJECTION_H_
#define PROJECTION_H_

#include <GL/glew.h>
#include "gl-matrix.h"

typedef struct {
	void (*init)();
	mat4_t (*get_orthographic_projection_matrix)();
	mat4_t (*get_prespective_projection_matrix)( GLfloat zoom_aspect );
	void (*clean)();
} Namespace_Projection;

extern Namespace_Projection const Projection;

#endif /* PROJECTION_H_ */
