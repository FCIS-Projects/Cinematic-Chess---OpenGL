/*
 * model.h
 *
 *  Created on: May 17, 2016
 *      Author: leo-0
 */

#ifndef MODEL_H_
#define MODEL_H_

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#ifndef PI
#define PI		3.14159265358979323846
#endif

#define TO_RADIAN(angle) (angle / 180.0f * PI )

class Model
{
private:
	glm::mat4 model_matrix;

public:
	Model();
	glm::mat4 get_model_matrix( GLfloat angle );
	virtual ~Model();
};

#endif /* MODEL_H_ */
