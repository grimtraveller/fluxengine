/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_frustum.h"
#include "flx_scene.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

inline void Frustum::extract(const Matrix4x4 &matView, const Matrix4x4 &matProj)
{
	Matrix4x4 comboMatrix = matView*matProj;

	// Left clipping plane
	m_planes[0].a = comboMatrix.m[3]	+ comboMatrix.m[0];
	m_planes[0].b = comboMatrix.m[7]	+ comboMatrix.m[4];
	m_planes[0].c = comboMatrix.m[11]	+ comboMatrix.m[8];
	m_planes[0].d = comboMatrix.m[15]	+ comboMatrix.m[12];
	// Right clipping plane
	m_planes[1].a = comboMatrix.m[3]	- comboMatrix.m[0];
	m_planes[1].b = comboMatrix.m[7]	- comboMatrix.m[4];
	m_planes[1].c = comboMatrix.m[11]	- comboMatrix.m[8];
	m_planes[1].d = comboMatrix.m[15]	- comboMatrix.m[12];
	// Top clipping plane
	m_planes[2].a = comboMatrix.m[3]	- comboMatrix.m[1];
	m_planes[2].b = comboMatrix.m[7]	- comboMatrix.m[5];
	m_planes[2].c = comboMatrix.m[11]	- comboMatrix.m[9];
	m_planes[2].d = comboMatrix.m[15]	- comboMatrix.m[13];
	// Bottom clipping plane
	m_planes[3].a = comboMatrix.m[3]	+ comboMatrix.m[1];
	m_planes[3].b = comboMatrix.m[7]	+ comboMatrix.m[5];
	m_planes[3].c = comboMatrix.m[11]	+ comboMatrix.m[9];
	m_planes[3].d = comboMatrix.m[15]	+ comboMatrix.m[13];
	// Near clipping plane
	m_planes[4].a = comboMatrix.m[3]	+ comboMatrix.m[2];
	m_planes[4].b = comboMatrix.m[7]	+ comboMatrix.m[6];
	m_planes[4].c = comboMatrix.m[11]	+ comboMatrix.m[10];
	m_planes[4].d = comboMatrix.m[15]	+ comboMatrix.m[14];
	// Far clipping plane
	m_planes[5].a = comboMatrix.m[3]	- comboMatrix.m[2];
	m_planes[5].b = comboMatrix.m[7]	- comboMatrix.m[6];
	m_planes[5].c = comboMatrix.m[11]	- comboMatrix.m[10];
	m_planes[5].d = comboMatrix.m[15]	- comboMatrix.m[14];

	for(int i = 0; i < 6; i++)
	{
		m_planes[i].normalize();
	}
}

inline bool Frustum::IsBoxInFrustum(const AABBox &box) const
{
	if(m_bIsActive)
	{
		Vector3 pt[8];
		box.getCorners(pt);

		for(int i = 0; i < 6; ++i)
		{
			if(m_planes[i].classifyPoint(pt[0]) == P_POSITIVE)
				continue;
			if(m_planes[i].classifyPoint(pt[1]) == P_POSITIVE)
				continue;
			if(m_planes[i].classifyPoint(pt[2]) == P_POSITIVE)
				continue;
			if(m_planes[i].classifyPoint(pt[3]) == P_POSITIVE)
				continue;
			if(m_planes[i].classifyPoint(pt[4]) == P_POSITIVE)
				continue;
			if(m_planes[i].classifyPoint(pt[5]) == P_POSITIVE)
				continue;
			if(m_planes[i].classifyPoint(pt[6]) == P_POSITIVE)
				continue;
			if(m_planes[i].classifyPoint(pt[7]) == P_POSITIVE)
				continue;

			return false;
		}
		return true;
	}
}

bool Frustum::IsVertexInFrustum(const Vector3& v) const
{
	if(m_bIsActive)
	{
		for(int i = 0; i < 6; i++)
		{
			if(m_planes[i].classifyPoint(v) == P_POSITIVE)
				continue;

			return false;
		}
		return true;
	}
}

inline bool Frustum::IsActive()
{ return m_bIsActive; }

void Frustum::EnableFrustum(bool b)
{ m_bIsActive = b; }