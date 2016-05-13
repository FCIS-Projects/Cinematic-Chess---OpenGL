/*
 * VRAM.c
 *
 *  Created on: May 13, 2016
 *      Author: leo-0
 */

#include "VRAM.h"
#include <assert.h>

DrawTypes draw_type = DYNAMIC_DRAW;

void init();
// TODO: add texture too
static GLuint send_data( VerticesData *vertices, VerticesIndicesData *indices,
		ColorData *colors, TextureData *textures );
static GLuint send_vertices_data( VerticesData *vertices_data );
static GLuint send_vertices_indices_data( VerticesIndicesData *indices_data );
static GLuint send_vertices_color_data( ColorData *colors_data );
void clean();

NameSpace_VRAM const VRAM = {
	init,
	send_data,
	clean
};

void init( DrawTypes __draw_type )
{
	draw_type = __draw_type;
}

GLuint send_data( VerticesData *vertices, VerticesIndicesData *indices,
		ColorData *colors, TextureData* textures )
{
	GLuint vertex_array_ID;

	// generate a vertex array object
	glGenVertexArrays(1, &vertex_array_ID);

	// bind the vertex array to configure it
	glBindVertexArray(vertex_array_ID);

	// send the vertices data to the memory
	send_vertices_data( vertices );

	if( indices != NULL )
	{
		// send the indices of the vertices data to the memory
		send_vertices_indices_data( indices );
	}


	if( colors != NULL )
	{
		// send the colors of the vertices data to the memory
		send_vertices_color_data( colors );
	}

/******************************* UNBINDING *******************************/
	/* Note that this is allowed, the call to glVertexAttribPointer
	* registered vertices_buffer_ID as the currently bound vertex buffer object
	* so afterwards we can safely unbind
	*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind vertex_array_ID (it's always a good thing to unbind any buffer/array
	// to prevent strange bugs)
	glBindVertexArray(0);


	return vertex_array_ID;
}

GLuint send_vertices_data( VerticesData *vertices_data )
{
	assert( vertices_data != NULL );
	assert( vertices_data->data != NULL );
	assert( vertices_data->length != 0 );

	GLuint buffer_ID;

	// vertex buffer object
	// generate a buffer object and save its ID to a variable
	// 1 -> one vertices_buffer_ID
	glGenBuffers(1, &buffer_ID);

	// binds that buffer object name to the target (GL_vertex_array_ID)
	// set vertices_buffer_ID as the current GL_vertex_array_ID.
	glBindBuffer(GL_ARRAY_BUFFER, buffer_ID);

	/* Copy "Array Buffer" from CPU-memory to GPU-buffer memory */
	/* arg 1: type of buffer
	 * arg 2: size of data in bytes
	 * arg 3: actual data
	 * arg 4: how we want the graphics card to manage the given data
	 * GL_STATIC_DRAW : the data will most likely not change at all or very rarely.
	 * GL_DYNAMIC_DRAW: the data is likely to change a lot.
	 * GL_STREAM_DRAW : the data will change every time it is drawn.
	 */
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices_data->length,
			vertices_data->data, draw_type);

	// Linking Vertex Attributes
	/*	GLuint index 		   -> layout (location = 0), so index = 0
	 *	GLint size  		   -> vec3, so size = 3
	 *	GLenum type            -> GL_FLOAT
	 *	GLboolean normalized   -> GL_TRUE -> all the data that has a value
	 *		not between 0 (or -1 for signed data) and 1
	 *	 	will be mapped to those values.
	 *	GLsizei stride 		   -> the space between
	 *		consecutive vertex attribute sets.
	 *	const GLvoid * pointer -> the offset of where the position data
	 *		begins in the buffer.
	 */
	glVertexAttribPointer( vertices_data->layout_location_in_shader,
						   3,
						   GL_FLOAT,
						   vertices_data->is_normalized,
						   3 * sizeof(GLfloat),
						   (GLvoid*)0 );

	// enable vertex attribute as it is disabled by default
	// argument #1: vertex attribute location
	glEnableVertexAttribArray(vertices_data->layout_location_in_shader);

	return buffer_ID;
}

GLuint send_vertices_indices_data( VerticesIndicesData *indices_data )
{
	assert( indices_data != NULL );
	assert( indices_data->data != NULL );
	assert( indices_data->length != 0 );

	GLuint buffer_ID;

	// vertex buffer object
	// generate a buffer object and save its ID to a variable
	// 1 -> one vertices_buffer_ID
	glGenBuffers(1, &buffer_ID);

	// binds that buffer object name to the target (GL_vertex_array_ID)
	// set vertices_buffer_ID as the current GL_vertex_array_ID.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_ID);

	/* Copy "Array Buffer" from CPU-memory to GPU-buffer memory */
	// arg 1: type of buffer
	// arg 2: size of data in bytes
	// arg 3: actual data
	// arg 4: how we want the graphics card to manage the given data
	// GL_STATIC_DRAW : the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
	// GL_STREAM_DRAW : the data will change every time it is drawn.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * indices_data->length,
			indices_data->data, draw_type);

	return buffer_ID;
}

GLuint send_vertices_color_data( ColorData *colors_data )
{
	assert( colors_data != NULL );
	assert( colors_data->data != NULL );
	assert( colors_data->length != 0 );

	GLuint buffer_ID;

	// vertex buffer object
	// generate a buffer object and save its ID to a variable
	// 1 -> one vertices_buffer_ID
	glGenBuffers(1, &buffer_ID);

	// binds that buffer object name to the target (GL_vertex_array_ID)
	// set vertices_buffer_ID as the current GL_vertex_array_ID.
	glBindBuffer(GL_ARRAY_BUFFER, buffer_ID);

	/* Copy "Array Buffer" from CPU-memory to GPU-buffer memory */
	// arg 1: type of buffer
	// arg 2: size of data in bytes
	// arg 3: actual data
	// arg 4: how we want the graphics card to manage the given data
	// GL_STATIC_DRAW : the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
	// GL_STREAM_DRAW : the data will change every time it is drawn.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors_data->length,
			colors_data->data, draw_type);

	// Linking Vertex Attributes
	/*	GLuint index 		   -> layout (location = 0), so index = 0
	 *	GLint size  		   -> vec3, so size = 3
	 *	GLenum type            -> GL_FLOAT
	 *	GLboolean normalized   -> GL_TRUE -> all the data that has a value
	 *		not between 0 (or -1 for signed data) and 1
	 *	 	will be mapped to those values.
	 *	GLsizei stride 		   -> the space between
	 *		consecutive vertex attribute sets.
	 *	const GLvoid * pointer -> the offset of where the position data
	 *		begins in the buffer.
	 */
	glVertexAttribPointer( colors_data->layout_location_in_shader,
						   3,
						   GL_FLOAT,
						   GL_FALSE,
						   3 * sizeof(GLfloat),
						   (GLvoid*)0 );

	// enable vertex attribute as it is disabled by default
	// argument #1: vertex attribute location
	glEnableVertexAttribArray(colors_data->layout_location_in_shader);

	return buffer_ID;
}

void clean()
{

}
