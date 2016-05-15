/*
 * Mmesh.cpp
 *
 *  Created on: May 13, 2016
 *      Author: leo-0
 */

#include "mesh.h"
#include <stdlib.h>

static void init();
//static void setup_mesh( Mesh *mesh );
static void clean();
static Mesh* create_mesh( VerticesData *vertices, VerticesIndicesData *indices,
		ColorData *colors, TextureData* textures );

Namespace_Mesh const MeshClass = {
	init,
	create_mesh,
//	setup_mesh,
	clean
};

void init()
{
	VRAM.init();
}

Mesh* create_mesh( VerticesData *vertices, VerticesIndicesData *indices,
		ColorData *colors, TextureData* textures )
{
	Mesh *mesh = malloc( sizeof(Mesh) );
	mesh->vertices = vertices;
	mesh->indices = indices;
	mesh->colors = colors;
	mesh->textures = textures;

	return mesh;
}

//void setup_mesh( Mesh *mesh )
//{
//	VRAM.send_data( mesh->vertices, mesh->indices, mesh->colors, mesh->textures);
//}

void clean()
{
	VRAM.clean();
}
