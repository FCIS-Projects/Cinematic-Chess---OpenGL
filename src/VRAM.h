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

// This struct hold the data of the textures
typedef struct
{
	size_t length;
	char* textures_paths[100];
	int* textures_unit_numbers;
} TextureData;

// This struct hold the data of the vertices
typedef struct
{
	size_t length;
	GLfloat* data;
	bool is_normalized;
	GLuint layout_location_in_shader;
} VerticesData;

// This struct hold the data of the indices of the vertices
typedef struct
{
	size_t length;
	GLuint* data;
} VerticesIndicesData;

// This struct hold the data of the colors of the vertices
typedef struct
{
	size_t length;
	GLfloat* data;
	GLuint layout_location_in_shader;
} ColorData;

typedef struct {
	void (*init)();
	// add new data to be drawn
	// where `VerticesData` must be defined ( with all its data )
	// and VerticesIndicesData, ColorData and TextureData are optional
	// which you can send them as `NULL`
	GLuint (*send_data)( VerticesData *vertices, VerticesIndicesData *indices,
			ColorData *colors, TextureData *textures );
	void (*clean)();

} NameSpace_VRAM;

extern NameSpace_VRAM const VRAM;

#endif /* VRAM_H_ */
