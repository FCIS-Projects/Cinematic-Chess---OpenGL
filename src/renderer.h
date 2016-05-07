/*
 * renderer.h
 *
 *  Created on: Mar 11, 2016
 *      Author: leo-0
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>
#include "bool.h"

//typedef enum BufferDataType
//{
//	ARRAY_BUFFER = 0,
//	ELEMENT_ARRAY_BUFFER,
//	TEXTURE_BUFFER
//} BufferDataType;

typedef enum {
	STATIC_DRAW,
	DYNAMIC_DRAW,
	STREAM_DRAW
} DrawTypes;

typedef struct
{
	size_t length;
	char* textures_paths[100];
	int* textures_unit_numbers;
} TextureData;

typedef struct
{
	size_t length;
	GLfloat* data;
	bool is_normalized;
	GLuint layout_location_in_shader;
} VerticesData;

typedef struct
{
	size_t length;
	GLuint* data;
} VerticesIndicesData;

typedef struct
{
	size_t length;
	GLfloat* data;
	GLuint layout_location_in_shader;
} ColorData;

typedef struct namesapce_GL_Renderer
{
	void (*init)( short view_width, short view_height, GLuint shader_program_ID );
	void (*set_type_of_draw)( DrawTypes type );
	GLuint (*add_data)( VerticesData *vertices, VerticesIndicesData *indices,
			ColorData *colors, TextureData *textures );
	void (*draw)();
	void (*set_view_coordinates)( short view_width, short view_height );
	void (*set_background_color)( GLfloat red, GLfloat green, GLfloat blue );
	void (*set_wireframe)(bool on);
	void (*clean)();
} namesapce_GL_Renderer;

extern namesapce_GL_Renderer const Renderer;

#endif /* RENDERER_H_ */
