/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../Core/flx_core.h"

class ENGINE_API Geometry
{
public:
	Geometry();
	virtual ~Geometry();

	virtual void Update() = 0;
	virtual void RenderOGL(bool bTextured) = 0;
	virtual char* GetTypeString() = 0;
	AABBox			m_AABBox;
	BoundingSphere	mBoundingSphere;

	bool	m_bRenderAABBox;
	bool	m_bSelected;
	Vector3 m_worldSpace;

	int		mNumVertices;
	bool	m_bRenderTextures;
};

#endif