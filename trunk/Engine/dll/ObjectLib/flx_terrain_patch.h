/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _TERRAIN_PATCH_H
#define _TERRAIN_PATCH_H

#include "../Core/flx_core.h"
#include "../MathLib/flx_vector3.h"
#include "../Core/flx_ogl_buffer.h"

class TerrainPatch
{
public:
	TerrainPatch();
	virtual ~TerrainPatch();

	void render();

	flx_ogl_buffer<flx_Vertex>		VertexBuffer;
	flx_ogl_buffer<flx_Normal>		NormalBuffer;
	flx_ogl_buffer<flx_TexCoord>	TexCoordBuffer;
	flx_ogl_buffer<flx_Index>		IndexBuffer;
	std::vector<unsigned int>		m_Indices[3];

	flx_Vertex		*patchVertices;
	flx_TexCoord	*patchTexCoords;
	flx_Normal		*patchNormals;

	unsigned int m_vertexCount;
	AABBox		m_aabb;
private:
};

#endif