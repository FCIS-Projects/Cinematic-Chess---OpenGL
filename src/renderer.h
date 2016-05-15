/*
 * renderer.h
 *
 *  Created on: Mar 11, 2016
 *      Author: leo-0
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>
#include "VRAM.h"
#include "mesh.h"
#include "bool.h"

//// this enum is used to define the way we want OpenGL draw our vertices
//typedef enum {
//	STATIC_DRAW,
//	DYNAMIC_DRAW,
//	STREAM_DRAW
//} DrawTypes;

// This struct hold the data of the textures
//typedef struct
//{
//	size_t length;
//	char* textures_paths[100];
//	int* textures_unit_numbers;
//} TextureData;
//
//// This struct hold the data of the vertices
//typedef struct
//{
//	size_t length;
//	GLfloat* data;
//	bool is_normalized;
//	GLuint layout_location_in_shader;
//} VerticesData;
//
//// This struct hold the data of the indices of the vertices
//typedef struct
//{
//	size_t length;
//	GLuint* data;
//} VerticesIndicesData;
//
//// This struct hold the data of the colors of the vertices
//typedef struct
//{
//	size_t length;
//	GLfloat* data;
//	GLuint layout_location_in_shader;
//} ColorData;

// this design is act like a namespace in C launguage
// this weird shape of function is a pointer of functions ( search about that )
// Usage:
// for example: Renderer.draw();
// init() is acting like constructor and clean() is acting like destructor
// to use Renderer you should first call init(), add you data throw
// add_data() and then invoke draw
// ( there will be another function where you set which objects to draw )
typedef struct namesapce_GL_Renderer
{
	// Constructor
	void (*init)( short view_width, short view_height, GLuint shader_program_ID );
	// set type of drawing according to the enum DrawTypes
	void (*set_type_of_draw)( DrawTypes type );
	// add new data to be drawn
	// where `VerticesData` must be defined ( with all its data )
	// and VerticesIndicesData, ColorData and TextureData are optional
	// which you can send them as `NULL`
	GLuint (*add_data)( VerticesData *vertices, VerticesIndicesData *indices,
			ColorData *colors, TextureData *textures );
	// DRAW
	void (*draw)();
	void (*draw_mesh)(Mesh *mesh);
	// set ViewPort coordinate
	void (*set_view_coordinates)( short view_width, short view_height );
	// change the background color of the viewport
	void (*set_background_color)( GLfloat red, GLfloat green, GLfloat blue );
	void (*set_wireframe)(bool on);
	// Destructor
	void (*clean)();
} namesapce_GL_Renderer;

extern namesapce_GL_Renderer const Renderer;

#endif /* RENDERER_H_ */
