/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef GEOMETRYMESH_H
#define GEOMETRYMESH_H

#include "flx_geometry.h"
#include "../Core/ResourceManager/flx_resource_mesh.h"

class ENGINE_API GeometryMesh : public Geometry
{
public:
	GeometryMesh(std::string _filename);
	virtual ~GeometryMesh();

	void Update();
	void RenderOGL(bool bTextured);
	char* GetTypeString() { return "Mesh"; }

	bool checkCollision(const Vector3& _point, Vector3 _offset, Vector3& _ret_offset);

	float m_fWidth;
	float m_fHeight;
	float m_fDepth;

	
	ResourceMesh2 pMesh;
};

#endif