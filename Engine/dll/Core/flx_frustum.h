/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "../Core/flx_core.h"
#include "../MathLib/flx_plane.h"
#include "../MathLib/flx_matrix4.h"



class ENGINE_API Frustum
{
public:
	Frustum();
	~Frustum();

	void extract(const Matrix4x4 &matView, const Matrix4x4 &matProj);

	bool IsBoxInFrustum(const AABBox& box) const;
	bool IsVertexInFrustum(const Vector3& v) const;
	bool IsActive();
	void EnableFrustum(bool b);

	Plane m_planes[6];
	float frustum[6][4];
private:
	bool m_bIsActive;
};

#endif