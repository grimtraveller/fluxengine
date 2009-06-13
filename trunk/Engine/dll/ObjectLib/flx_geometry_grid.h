/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef GEOMETRYFLATGRID_H
#define GEOMETRYFLATGRID_H

#include "../Core/flx_core.h"
#include "../ObjectLib/flx_geometry.h"
#include "../ObjectLib/flx_vertex3.h"
#include "../ObjectLib/flx_vertex2.h"
#include "../MathLib/flx_vector3.h"

class ENGINE_API GeometryFlatGrid : public Geometry
{
public:
	GeometryFlatGrid(float fWidth, float fDepth, int iDivX, int iDivZ);
	virtual ~GeometryFlatGrid();

	void Update();
	void RenderOGL(bool bTexture);
	char* GetTypeString() { return "FlatGrid"; }

protected:

public:

private:
	Vertex3* m_V;
	Vertex2* m_T; //texture coordinates

	float m_fWidth;
	float m_fDepth;
	int m_iDivX;
	int m_iDivZ;
};

#endif