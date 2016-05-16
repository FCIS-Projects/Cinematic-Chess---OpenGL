/*
 * VRAM.h
 *
 *  Created on: May 13, 2016
 *      Author: leo-0
 */

#ifndef VRAM_H_
#define VRAM_H_

#include <GL/glew.h>
#include "bool.h"

// this enum is used to define the way we want OpenGL draw our vertices
typedef enum {
	STATIC_DRAW,
	DYNAMIC_DRAW,
	STREAM_DRAW
} DrawTypes;

typedef struct
{
	size_t length;
	GLuint *data;
} VAOs;

// This struct hold the data of the textures
typedef struct
{
	// length of the texture data
	size_t length;
	// array of `string` holds the pathes of the textures
	char* textures_paths[100];
	// array of `int` holds the unit number of textures
	int* textures_unit_numbers;
} TextureData;

// This struct hold the data of the vertices
typedef struct
{
	// length of vertices data
	size_t length;
	// array of floats that holds vertices data
	GLfloat* data;
	// if the vertices are normalized ( their range are between -1 and 1 )
	bool is_normalized;
	// layout location in GLSL shader
	GLuint layout_location_in_shader;
} VerticesData;

// This struct hold the data of the indices of the vertices
typedef struct
{
	// length of indices of the vertices data
	size_t length;
	// the indices of the vertices
	GLuint* data;
} VerticesIndicesData;

// This struct hold the data of the colors of the vertices
typedef struct
{
	// the length of the color data
	size_t length;
	// the colors of the vertices
	GLfloat* data;
	// layout location in GLSL shader
	GLuint layout_location_in_shader;
} ColorData;

typedef struct
{
	// length of normals of the vertices data
	size_t length;
	// the normals of the vertices
	GLuint* data;
	// layout location in GLSL shader
	GLuint layout_location_in_shader;
} NormalsData;

/* This struct is acting like namespace and class like in OOP at the sametime!
 * to use it:
 * for example: VRAM.init();
 */
typedef struct {
	// this acts like constructor
	void (*init)();
	// add new data to be drawn
	// where `VerticesData` must be defined ( with all its data )
	// and VerticesIndicesData, ColorData and TextureData are optional
	// which you can send them as `NULL`
	GLuint (*send_data)( VerticesData *vertices, VerticesIndicesData *indices,
			ColorData *colors, TextureData *textures, NormalsData *normals );
//	VAOs* (*get_active_VAOs)();
	// this acts like destructor
	void (*clean)();

} NameSpace_VRAM;

extern NameSpace_VRAM const VRAM;

#endif /* VRAM_H_ */
