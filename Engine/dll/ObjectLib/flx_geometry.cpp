/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_geometry.h"

Geometry::Geometry()
{
	m_bRenderAABBox		= false;
	m_bSelected			= false;
	m_bRenderTextures	= true;

	mBoundingSphere.setCenter(m_AABBox);
}

Geometry::~Geometry()
{
}
