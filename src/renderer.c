/*
 * renderer.c
 *
 *  Created on: Mar 11, 2016
 *  Author: Baron Leonardo
 */

#include "renderer.h"
//#include "shader.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "gl-matrix.h"

// random
#include <time.h>
#include <stdlib.h>

#define BG_RED     0.0f
#define BG_GREEN   0.0f
#define BG_BLUE    0.0f
#define BG_OPACITY 1.0f

short _view_width  = 800;
short _view_height = 600;

GLuint draw_type = GL_DYNAMIC_DRAW;
GLuint _shader_program_ID = 0;

// Our vertices
//GLfloat vertices_cube[] = {
//	-0.5f, -0.5f,  -0.5f,	0.0, 0.0, 0.0,
//	-0.5f, -0.5f,  0.5f,	0.0, 0.0, 1.0,
//	-0.5f,  0.5f,  -0.5f,	0.0, 1.0, 0.0,
//	-0.5f,  0.5f,  0.5f,	0.0, 1.0, 1.0,
//	 0.5f, -0.5f,  -0.5f,	1.0, 0.0, 0.0,
//	 0.5f, -0.5f,  0.5f,	1.0, 0.0, 1.0,
//	 0.5f,  0.5f,  -0.5f,	1.0, 1.0, 0.0,
//	 0.5f,  0.5f,  0.5f,	1.0, 1.0, 1.0,
//};
//
//// Our indices
//GLuint indices_cube[] = {
//    0, 1, 2,
//    2, 3, 1,
//
//    4, 5, 6,
//    6, 7, 5,
//
//    1, 5, 3,
//    3, 7, 5,
//
//    4, 0, 6,
//    6, 2, 0,
//
//    6, 2, 7,
//    7, 3, 2,
//
//    4, 0, 5,
//    5, 1, 0,
//};

// vertex buffer object
//GLuint vertices_buffer_ID = 0;
// element buffer object
//GLuint indices_buffer_ID = 0;
// vertex array object
//GLuint vertex_array_ID = 0;

mat4_t model_matrix;
mat4_t view_matrix;
mat4_t projection_matrix;

GLint model_uniform = -1;
GLint view_uniform = -1;
GLint projection_uniform = -1;

// TODO: shaders paths
char* _vertex_shader_path;
char* _fragment_shader_path;

typedef struct
{
	size_t length;
	GLuint *data;
} Buffers;

Buffers *VAOs = NULL;

static void init( short view_width, short view_height, GLuint shader_program_ID );
static void draw( GLuint vertex_array_ID );
static void set_view_coordinates( short view_width, short view_height );
static void set_background_color( GLfloat red, GLfloat green, GLfloat blue );
static void set_wireframe(bool on);
static void set_type_of_draw( DrawTypes type );
// TODO: add texture too
static GLuint add_data( VerticesData *vertices, VerticesIndicesData *indices,
		ColorData *colors, TextureData textures );
static GLuint send_vertices_data_to_VRAM( VerticesData *data );
static GLuint send_vertices_indices_data_to_VRAM( VerticesIndicesData *indices_data);
static GLuint send_vertices_color_data_to_VRAM( ColorData *colors );
//static void send_data_to_GPU_inner( GLfloat data[], size_t length,
//		BufferDataType type, GLuint layout_location_in_shader,
//		GLboolean is_normalized );
static void init_uniform();
static void transformation();
static void clean();

namesapce_GL_Renderer const Renderer = {
		init,
		set_type_of_draw,
		add_data,
		draw,
		set_view_coordinates,
		set_background_color,
		set_wireframe,
		clean };

void init( short view_width, short view_height,  GLuint shader_program_ID )
{
/* first  : generate vertices_buffer_IDs vertex_array_IDs indices_buffer_IDs
 * second : binding
 * third  : buffer data ( copy array buffer from CPU to GPU ) vertices_buffer_IDs VEOx
 * forth  : glVertexAttribPointer linking to vertex attrib ( in shaders )
 * fifth  : enable vertex attribute pointer
 * sixth  : unbind vertices_buffer_IDs vertex_array_IDs
 */

	_view_width 	   = view_width;
	_view_height 	   = _view_height;
	_shader_program_ID = shader_program_ID;

	// initialize array of VAO
	VAOs = malloc( sizeof(Buffers) );
	VAOs->data = malloc( sizeof(size_t) * ( GL_MAX_VERTEX_ATTRIBS - 1 ) );
	VAOs->length = 0;


/******************************** SHADER ********************************/
	// create vertex shader, fragment shader and a shader program
	// TODO: Shader
//	Shader.create_vertex_shader(_vertex_shader_path);
//	Shader.create_fragment_shader(_fragment_shader_path);
//	Shader.create_shader_program();

/********************************* CLEAR *********************************/
	// set a clear color
	glClearColor( BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY );

/******************************** GENERATE ******************************/
	// generate a vertex array object
//	glGenVertexArrays(1, &vertex_array_ID);

	// vertex buffer object
	// generate a buffer object and save its ID to a variable
	// 1 -> one vertices_buffer_ID
//	glGenBuffers(1, &vertices_buffer_ID);

	// element buffer object
	// use to not duplicate vertecies in case we want to draw traiangle
	// that overlapped at some verteces
//	glGenBuffers(1, &indices_buffer_ID);

/******************************* BINDING *******************************/
//	glBindVertexArray(vertex_array_ID);

	// binds that buffer object name to the target (GL_vertex_array_ID)
	// set vertices_buffer_ID as the current GL_vertex_array_ID.
//	glBindBuffer(GL_vertex_array_ID, vertices_buffer_ID);

//	glBindBuffer(GL_ELEMENT_vertex_array_ID, indices_buffer_ID);

/***************************** BUFFER DATA *****************************/
	/* Copy "Array Buffer" from CPU-memory to GPU-buffer memory */
	// arg 1: type of buffer
	// arg 2: size of data in bytes
	// arg 3: actual data
	// arg 4: how we want the graphics card to manage the given data
	// GL_STATIC_DRAW : the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
	// GL_STREAM_DRAW : the data will change every time it is drawn.
//	glBufferData(GL_vertex_array_ID, sizeof(vertices_cube),
//				vertices_cube, GL_STATIC_DRAW);

//	glBufferData(GL_ELEMENT_vertex_array_ID, sizeof(indices_cube),
//			indices_cube, GL_STATIC_DRAW);

/************************** VERTEX ATTRIBUTES **************************/
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
//	glVertexAttribPointer( 0,
//						   3,
//						   GL_FLOAT,
//						   GL_FALSE,
//						   6 * sizeof(GLfloat),
//						   (GLvoid*)0 );

	// enable vertex attribute as it is disabled by default
	// argument #1: vertex attribute location
//	glEnableVertexAttribArray(0);

//	glVertexAttribPointer( 1,
//						   3,
//						   GL_FLOAT,
//						   GL_FALSE,
//						   6 * sizeof(GLfloat),
//						   (GLvoid*)(3 * sizeof(GLfloat)) );

//	glEnableVertexAttribArray(1);

/********************************* MODES *********************************/
//	set_wireframe(true);

/******************************* UNBINDING *******************************/
	/* Note that this is allowed, the call to glVertexAttribPointer
	* registered vertices_buffer_ID as the currently bound vertex buffer object
	* so afterwards we can safely unbind
	*/
//    glBindBuffer(GL_vertex_array_ID, 0);

    // Unbind vertex_array_ID (it's always a good thing to unbind any buffer/array
    // to prevent strange bugs)
//    glBindVertexArray(0);

/******************************** UNIFORMS ********************************/
    init_uniform();

/********************************* Camera *********************************/
}

//void send_data_to_GPU_inner( GLfloat data[], size_t length,
//		BufferDataType type, GLuint layout_location_in_shader,
//		GLboolean is_normalized )
//{

//}


GLuint add_data( VerticesData *vertices, VerticesIndicesData *indices,
		ColorData *colors, TextureData textures )
{
	GLuint vertex_array_ID;
	GLuint vertices_buffer_ID;
	GLuint color_buffer_ID;
	GLuint indices_buffer_ID;

/******************************** GENERATE ******************************/
	// generate a vertex array object
	glGenVertexArrays(1, &vertex_array_ID);

	// add the new VAO to the array
	VAOs->data[ VAOs->length ] = vertex_array_ID;
	(VAOs->length)++;

/******************************* BINDING *******************************/
	glBindVertexArray(vertex_array_ID);

/***************************** BUFFER DATA *****************************/
	vertices_buffer_ID = send_vertices_data_to_VRAM( vertices );

/**************************** Element Buffer ****************************/
	if( indices != NULL )
	{
		indices_buffer_ID = send_vertices_indices_data_to_VRAM( indices );
	}

/**************************** Color Buffer ****************************/
	if( colors != NULL )
	{
		indices_buffer_ID = send_vertices_color_data_to_VRAM( colors );
	}

/********************************* MODES *********************************/
//	set_wireframe(true);

/******************************* UNBINDING *******************************/
	/* Note that this is allowed, the call to glVertexAttribPointer
	* registered vertices_buffer_ID as the currently bound vertex buffer object
	* so afterwards we can safely unbind
	*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind vertex_array_ID (it's always a good thing to unbind any buffer/array
	// to prevent strange bugs)
	glBindVertexArray(0);

/******************************** UNIFORMS ********************************/
	init_uniform();

	return vertex_array_ID;
}

GLuint send_vertices_data_to_VRAM( VerticesData *data )
{
	assert( data != NULL );
	assert( data->data != NULL );

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data->length,
				data, draw_type);

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
	glVertexAttribPointer( data->layout_location_in_shader,
						   3,
						   GL_FLOAT,
						   data->is_normalized,
						   3 * sizeof(GLfloat),
						   (GLvoid*)0 );

	return buffer_ID;
}

GLuint send_vertices_indices_data_to_VRAM( VerticesIndicesData *data )
{
	GLuint buffer_ID;

	glGenBuffers(1, &buffer_ID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_ID);

	/* Copy "Array Buffer" from CPU-memory to GPU-buffer memory */
	// arg 1: type of buffer
	// arg 2: size of data in bytes
	// arg 3: actual data
	// arg 4: how we want the graphics card to manage the given data
	// GL_STATIC_DRAW : the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
	// GL_STREAM_DRAW : the data will change every time it is drawn.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * data->length,
				data, draw_type);

	return buffer_ID;
}

GLuint send_vertices_color_data_to_VRAM( ColorData *colors )
{
	assert( colors != NULL );
	assert( colors->data != NULL );

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors->length,
				colors->data, draw_type);

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
	glVertexAttribPointer( colors->layout_location_in_shader,
						   3,
						   GL_FLOAT,
						   GL_FALSE,
						   3 * sizeof(GLfloat),
						   (GLvoid*)0 );

	return buffer_ID;
}

void set_active_VAOs(  )
{

}

void draw()
{
	// TODO: make sure we have VAOs
	// prushing the background with a color
	// calculate Z-buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// TODO: Use Shader Program
//	Shader.use_shader_program();

/******************************** UNIFORMS ********************************/
//	fading_color_using_uniform();
	transformation();

	/* Update uniform (fragment color)
	 * arg #1 : uniform location
	 * arg #2 : the value(s)
	 */
//	glUniform4f(uniform_location, 0.0f, green, 0.0f, 1.0f);
//	glUniform1f( uniform_location, 0.5f );

	/* Update uniform (fragment color)
	 * arg #1 : uniform location
	 * arg #2 : the number of elements [matrices]
	 * arg #3 : true  -> need to be transposed - the default in GLSL that it
	 * 						takes column based matrix instead of row one -
	 * 			false -> don't need to be transposed
	 * arg #4 : pointer to the matrix array
	 */
	glUniformMatrix4fv(model_uniform, 1, GL_FALSE, model_matrix);
	glUniformMatrix4fv(view_uniform, 1, GL_FALSE, view_matrix);
	glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, projection_matrix);

/**************************************************************************/

	/* draws primitives using the currently active shader,
	 * the previously defined vertex attribute configuration
	 * and with the vertices_buffer_ID's vertex data (indirectly bound via the vertex_array_ID)
	 *
	 * arg #1: primitive type
	 * arg #2: start index of the vertex_array_ID we want to draw
	 * arg #3: the number of vertices
	 */
	// glDrawArrays(GL_TRIANGLES, 0, 3);

	if(glGetError() == GL_INVALID_OPERATION )
	{
		fprintf(stderr, "Did you use `glUseProgram(shader_program)` ?\n");
	}

	for (int iii = 0; iii < VAOs->length; ++iii)
	{
		// BIND (enable this vertex array to be used)
		glBindVertexArray(VAOs->data[iii]);

		/* arg #1: primitive type
		 * arg #2: number of elements (verteces) we want to draw
		 * arg #3: type of indices
		 * arg #4: the offset between indices
		 */
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0) );
	}


	// UNBIND
	glBindVertexArray(0);
}

void clean()
{
	// free all the VAOs
	glDeleteVertexArrays(VAOs->length, VAOs->data);

	free(VAOs->data);
	free(VAOs);
	VAOs = NULL;

	// TODO: Clean Shader
//	Shader.clean();
}

void init_uniform()
{
	/*
	 * First  : there should be a uinform variable in a shader file
	 * Second : get the uniform variable
	 * Third  : to set unifrom it should done after glUseProgram() as it uses
	 * 			the active shader program
	 */

	const char model[] = "model";
	const char view[] = "view";
	const char projection[] = "projection";

	// arg #1: shader program
	// arg #2: shader name in string type
	// TODO: get shader Program ID
	model_uniform = glGetUniformLocation(_shader_program_ID,
			model );
	view_uniform = glGetUniformLocation(_shader_program_ID,
				view );
	projection_uniform = glGetUniformLocation(_shader_program_ID,
				projection );

	if( model_uniform == -1 || view_uniform == -1 || projection_uniform == -1 )
	{
		fprintf( stderr, "Couldn't find \"%s\"\n", model );
	}
}

void transformation()
{
	model_matrix = mat4_identity(NULL);
	view_matrix = mat4_identity(NULL);
	projection_matrix = mat4_create(NULL);

//	GLfloat cubePositions[][3] = {
//			{ 0.0f,  0.0f,  0.0f },
//			{ 2.0f,  5.0f, -15.0f },
//			{-1.5f, -2.2f, -2.5f },
//			{-3.8f, -2.0f, -12.3f },
//			{ 2.4f, -0.4f, -3.5f },
//			{-1.7f,  3.0f, -7.5f },
//			{ 1.3f, -2.0f, -2.5f },
//			{ 1.5f,  2.0f, -2.5f },
//			{ 1.5f,  0.2f, -1.5f },
//			{-1.3f,  1.0f, -1.5f },
//	};

	/* Model Matrix - Rotatation */
	vec3_t vec_rot = vec3_create(NULL);
	vec_rot[0] = 0.5f; vec_rot[1] = 1.0f; vec_rot[2] = 0.0f;
	mat4_rotate( model_matrix, (GLfloat)glfwGetTime() *55.0f * M_PI/180.0f,
			vec_rot, model_matrix );

	// TODO: get view matric
//	view_matrix = Camera.get_view_matrix();

	/* Build prespective projection */
	/* It used in the for the projection matrix
	 * arg #1: FOV - field of view - angle
	 * arg #2: aspect ratio - width / height
	 * arg #3: distance between the near plane and the camera
	 * arg #4: distance between the far plane and the camera
	 */
	// TODO: get camera zoom aspect
//	mat4_perspective( Camera.get_zoom_aspect(), screen_width / screen_height,
//									0.1f, 100.0f, projection_matrix );
}

void set_view_coordinates( short view_width, short view_height )
{
	_view_width = view_width;
	_view_height = _view_height;
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
