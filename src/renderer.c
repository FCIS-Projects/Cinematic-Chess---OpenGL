/*
 * renderer.c
 *
 *  Created on: Mar 11, 2016
 *  Author: Baron Leonardo
 */

#include "renderer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mesh.h"
#include <GLFW/glfw3.h>

#define BG_RED     0.0f
#define BG_GREEN   0.0f
#define BG_BLUE    0.0f
#define BG_OPACITY 1.0f

static short _view_width;
static short _view_height;

static GLuint draw_type;
static GLuint _shader_program_ID;

static VAOs *__VAOs = NULL;
static GLuint __current_VAO_ID;
static Mesh *meshes = NULL;

static void init( short view_width, short view_height, GLuint shader_program_ID );
static void draw();
static void draw_mesh(Mesh *mesh);
static void draw_triangles();
static void set_view_coordinates( short view_width, short view_height );
static void set_background_color( GLfloat red, GLfloat green, GLfloat blue );
static void set_wireframe(bool on);
static void set_type_of_draw( DrawTypes type );
// TODO: add texture too
static GLuint add_data( VerticesData *vertices, VerticesIndicesData *indices,
		ColorData *colors, TextureData *textures, NormalsData *normals );
//static GLuint send_vertices_data_to_VRAM( VerticesData *vertices_data );
//static GLuint send_vertices_indices_data_to_VRAM( VerticesIndicesData *indices_data);
//static GLuint send_vertices_color_data_to_VRAM( ColorData *colors_data );
static void clean();

namesapce_GL_Renderer const Renderer = {
		init,
		set_type_of_draw,
		add_data,
		draw,
		draw_mesh,
		set_view_coordinates,
		set_background_color,
		set_wireframe,
		clean };

void init( short view_width, short view_height,  GLuint shader_program_ID )
{
	// Intialize the data
	_view_width 	   = view_width;
	_view_height 	   = view_height;
	_shader_program_ID = shader_program_ID;

	draw_type = GL_DYNAMIC_DRAW;

	// initialize array of VAO
	__VAOs = malloc( sizeof(VAOs) );
	__VAOs->data = malloc( sizeof(size_t) * ( GL_MAX_VERTEX_ATTRIBS - 1 ) );
	__VAOs->length = 0;

	meshes = malloc(sizeof(Mesh) * ( GL_MAX_VERTEX_ATTRIBS - 1 ) );

	// set the background color
	glClearColor( BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY );

	// init `VRAM`
	// TODO: we need a separate function to handle draw type
	// TODO: we need to add it as a parameter
	VRAM.init(GL_STATIC_DRAW);
}

GLuint add_data( VerticesData *vertices, VerticesIndicesData *indices,
		ColorData *colors, TextureData* textures, NormalsData *normals )
{
	// check if glData is un-intialized
	assert(meshes != NULL);
//
	GLuint vertex_array_ID;

	vertex_array_ID = VRAM.send_data( vertices, indices, colors, textures, normals );
//
//	// generate a vertex array object
//	glGenVertexArrays(1, &vertex_array_ID);

	// add the new VAO to the array
	__VAOs->data[ __VAOs->length ] = vertex_array_ID;
	(__VAOs->length)++;

	// TODO:
	// append new VAO data into meshes where index equals
	// the generated vertex_array_ID
	meshes[vertex_array_ID].vertices = vertices;
	meshes[vertex_array_ID].indices = indices;
	meshes[vertex_array_ID].textures = textures;
	meshes[vertex_array_ID].colors = colors;
//
//	// bind the vertex array to configure it
//	glBindVertexArray(vertex_array_ID);
//
//	// send the vertices data to the memory
//	send_vertices_data_to_VRAM( vertices );
//
//	if( indices != NULL )
//	{
//		// send the indices of the vertices data to the memory
//		send_vertices_indices_data_to_VRAM( indices );
//	}
//
//
//	if( colors != NULL )
//	{
//		// send the colors of the vertices data to the memory
//		send_vertices_color_data_to_VRAM( colors );
//	}
//
///******************************* UNBINDING *******************************/
//	/* Note that this is allowed, the call to glVertexAttribPointer
//	* registered vertices_buffer_ID as the currently bound vertex buffer object
//	* so afterwards we can safely unbind
//	*/
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// Unbind vertex_array_ID (it's always a good thing to unbind any buffer/array
//	// to prevent strange bugs)
//	glBindVertexArray(0);

	return vertex_array_ID;
}

//GLuint send_vertices_data_to_VRAM( VerticesData *vertices_data )
//{
//	assert( vertices_data != NULL );
//	assert( vertices_data->data != NULL );
//	assert( vertices_data->length != 0 );
//
//	GLuint buffer_ID;
//
//	// vertex buffer object
//	// generate a buffer object and save its ID to a variable
//	// 1 -> one vertices_buffer_ID
//	glGenBuffers(1, &buffer_ID);
//
//	// binds that buffer object name to the target (GL_vertex_array_ID)
//	// set vertices_buffer_ID as the current GL_vertex_array_ID.
//	glBindBuffer(GL_ARRAY_BUFFER, buffer_ID);
//
//	/* Copy "Array Buffer" from CPU-memory to GPU-buffer memory */
//	/* arg 1: type of buffer
//	 * arg 2: size of data in bytes
//	 * arg 3: actual data
//	 * arg 4: how we want the graphics card to manage the given data
//	 * GL_STATIC_DRAW : the data will most likely not change at all or very rarely.
//	 * GL_DYNAMIC_DRAW: the data is likely to change a lot.
//	 * GL_STREAM_DRAW : the data will change every time it is drawn.
//	 */
//	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices_data->length,
//			vertices_data->data, draw_type);
//
//	// Linking Vertex Attributes
//	/*	GLuint index 		   -> layout (location = 0), so index = 0
//	 *	GLint size  		   -> vec3, so size = 3
//	 *	GLenum type            -> GL_FLOAT
//	 *	GLboolean normalized   -> GL_TRUE -> all the data that has a value
//	 *		not between 0 (or -1 for signed data) and 1
//	 *	 	will be mapped to those values.
//	 *	GLsizei stride 		   -> the space between
//	 *		consecutive vertex attribute sets.
//	 *	const GLvoid * pointer -> the offset of where the position data
//	 *		begins in the buffer.
//	 */
//	glVertexAttribPointer( vertices_data->layout_location_in_shader,
//						   3,
//						   GL_FLOAT,
//						   vertices_data->is_normalized,
//						   3 * sizeof(GLfloat),
//						   (GLvoid*)0 );
//
//	// enable vertex attribute as it is disabled by default
//	// argument #1: vertex attribute location
//	glEnableVertexAttribArray(vertices_data->layout_location_in_shader);
//
//	return buffer_ID;
//}
//
//GLuint send_vertices_indices_data_to_VRAM( VerticesIndicesData *indices_data )
//{
//	assert( indices_data != NULL );
//	assert( indices_data->data != NULL );
//	assert( indices_data->length != 0 );
//
//	GLuint buffer_ID;
//
//	// vertex buffer object
//	// generate a buffer object and save its ID to a variable
//	// 1 -> one vertices_buffer_ID
//	glGenBuffers(1, &buffer_ID);
//
//	// binds that buffer object name to the target (GL_vertex_array_ID)
//	// set vertices_buffer_ID as the current GL_vertex_array_ID.
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_ID);
//
//	/* Copy "Array Buffer" from CPU-memory to GPU-buffer memory */
//	// arg 1: type of buffer
//	// arg 2: size of data in bytes
//	// arg 3: actual data
//	// arg 4: how we want the graphics card to manage the given data
//	// GL_STATIC_DRAW : the data will most likely not change at all or very rarely.
//	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
//	// GL_STREAM_DRAW : the data will change every time it is drawn.
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * indices_data->length,
//			indices_data->data, draw_type);
//
//	return buffer_ID;
//}
//
//GLuint send_vertices_color_data_to_VRAM( ColorData *colors_data )
//{
//	assert( colors_data != NULL );
//	assert( colors_data->data != NULL );
//	assert( colors_data->length != 0 );
//
//	GLuint buffer_ID;
//
//	// vertex buffer object
//	// generate a buffer object and save its ID to a variable
//	// 1 -> one vertices_buffer_ID
//	glGenBuffers(1, &buffer_ID);
//
//	// binds that buffer object name to the target (GL_vertex_array_ID)
//	// set vertices_buffer_ID as the current GL_vertex_array_ID.
//	glBindBuffer(GL_ARRAY_BUFFER, buffer_ID);
//
//	/* Copy "Array Buffer" from CPU-memory to GPU-buffer memory */
//	// arg 1: type of buffer
//	// arg 2: size of data in bytes
//	// arg 3: actual data
//	// arg 4: how we want the graphics card to manage the given data
//	// GL_STATIC_DRAW : the data will most likely not change at all or very rarely.
//	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
//	// GL_STREAM_DRAW : the data will change every time it is drawn.
//	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors_data->length,
//			colors_data->data, draw_type);
//
//	// Linking Vertex Attributes
//	/*	GLuint index 		   -> layout (location = 0), so index = 0
//	 *	GLint size  		   -> vec3, so size = 3
//	 *	GLenum type            -> GL_FLOAT
//	 *	GLboolean normalized   -> GL_TRUE -> all the data that has a value
//	 *		not between 0 (or -1 for signed data) and 1
//	 *	 	will be mapped to those values.
//	 *	GLsizei stride 		   -> the space between
//	 *		consecutive vertex attribute sets.
//	 *	const GLvoid * pointer -> the offset of where the position data
//	 *		begins in the buffer.
//	 */
//	glVertexAttribPointer( colors_data->layout_location_in_shader,
//						   3,
//						   GL_FLOAT,
//						   GL_FALSE,
//						   3 * sizeof(GLfloat),
//						   (GLvoid*)0 );
//
//	// enable vertex attribute as it is disabled by default
//	// argument #1: vertex attribute location
//	glEnableVertexAttribArray(colors_data->layout_location_in_shader);
//
//	return buffer_ID;
//}

void draw()
{
	// make sure we have VAOs
	assert( __VAOs != NULL );

	// prushing the background with a color
	// calculate Z-buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// check if there is an active shader program
	if(glGetError() == GL_INVALID_OPERATION )
	{
		fprintf(stderr, "Did you use `glUseProgram(shader_program)` ?\n");
	}

	// draw the objects as group of triangles
	draw_triangles();
}

void draw_mesh(Mesh *mesh)
{
	add_data( mesh->vertices, mesh->indices, mesh->colors, mesh->textures,
			mesh->normals );
	// TODO: we need to draw only this Mesh ? or the whole scene
	draw();
}

void draw_triangles()
{
	// get active VAOs to draw them
//	__VAOs = VRAM.get_active_VAOs();

	for (int iii = 0; iii < __VAOs->length; ++iii)
	{
		__current_VAO_ID = __VAOs->data[iii];

		// BIND (enable this vertex array to be used)
		glBindVertexArray(__current_VAO_ID);

		if( meshes[__current_VAO_ID].indices != NULL )
		{
			/* arg #1: primitive type
			 * arg #2: number of elements (verteces) we want to draw
			 * arg #3: type of indices
			 * arg #4: the offset between indices
			 */
			glDrawElements(GL_TRIANGLES, meshes[__current_VAO_ID].indices->length,
					GL_UNSIGNED_INT, (void*)(0) );
		}

		else
		{
			for (int jjj = 0; jjj < meshes[__current_VAO_ID].vertices->length; jjj += 3)
			{
				/* draws primitives using the currently active shader,
				 * the previously defined vertex attribute configuration
				 * and with the VBO's vertex data (indirectly bound via the VAO)
				 *
				 * arg #1: primitive type
				 * arg #2: start index of the VAO we want to draw
				 * arg #3: the number of vertices
				 */
				glDrawArrays(GL_TRIANGLES, jjj, 3);
			}
		}

		// UNBIND
		glBindVertexArray(0);
	}
}

void set_view_coordinates( short view_width, short view_height )
{
	_view_width = view_width;
	_view_height = view_height;
}

void set_background_color( GLfloat red, GLfloat green, GLfloat blue )
{
	glClearColor( red, green, blue, BG_OPACITY );
}

void set_wireframe(bool on)
{
	if(on)
	{
		// wireframe mode
		/* arg #1: apply it to the front and back of all triangles
		 * arg #2: draw as lines
		 */
		 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	else
	{
		// Default mode
		 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void set_type_of_draw( DrawTypes type )
{
	switch( type )
	{
		case STATIC_DRAW:
			draw_type = GL_STATIC_DRAW;
			break;
		case DYNAMIC_DRAW:
			draw_type = GL_DYNAMIC_DRAW;
			break;
		case STREAM_DRAW:
			draw_type = GL_STREAM_DRAW;
			break;
		default:
			fprintf(stderr, "This is not a draw type!");
	};
}

void clean()
{
	// free all the VAOs
	glDeleteVertexArrays(__VAOs->length, __VAOs->data);

	free(__VAOs->data);
	free(__VAOs);
	__VAOs = NULL;

	VRAM.clean();

	free(meshes);
	meshes = NULL;
}
