/*
 * model.c
 *
 *  Created on: May 17, 2016
 *      Author: leo-0
 */

#include "model.h"

Model::Model()
{

}

glm::mat4 Model::get_model_matrix( GLfloat angle )
{
	angle = TO_RADIAN(angle);

	this->model_matrix = glm::rotate(this->model_matrix, angle,
			glm::vec3(1.0f, 0.3f, 0.5f));

	return this->model_matrix;
}

Model::~Model()
{

}
