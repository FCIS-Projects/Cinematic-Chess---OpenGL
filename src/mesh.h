/*
 * mesh.h
 *
 *  Created on: May 13, 2016
 *      Author: leo-0
 */

#ifndef MESH_H_
#define MESH_H_

#include "VRAM.h"

typedef struct{
	VerticesData *vertices;
	VerticesIndicesData *indices;
	ColorData *colors;
	TextureData *textures;
	NormalsData *normals;
} Mesh;

typedef struct {
	void (*init)();
	Mesh* (*create_mesh)( VerticesData *vertices, VerticesIndicesData *indices,
			ColorData *colors, TextureData* textures, NormalsData *normals );
//	void (*setup_mesh)(Mesh *mesh);
	void (*clean)();
} Namespace_Mesh ;

extern Namespace_Mesh const MeshClass;

#endif /* MESH_H_ */
