/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef PLANE_H
#define PLANE_H

#include "../Core/flx_core.h"
#include "flx_math.h"
#include "flx_vector3.h"

enum location_to_plane
{
	P_NEGATIVE,
	P_POSITIVE,
	P_ONPLANE
};

class Plane
{
public:
	inline Plane()
	{
		m_v3PositionVector	= Vector3(0, 0, 0);
		m_v3Normal			= Vector3(0, 1, 0);

		m_bOk = true;
	}

	inline Plane(const Vector3& v3Point, const Vector3& v3Normal)
		: m_v3PositionVector(v3Point), m_v3Normal(v3Normal)
	{
		m_fDist				= dot(m_v3Normal, v3Point);


		if(m_v3Normal.isZero())
		{
			m_bOk				= false;
			return;
		}

		m_bOk = true;

		m_v3Normal.normalize();
		m_fDist /= m_v3Normal.fMag;
	}

	inline Plane(const Vector3& v3Point1, const Vector3& v3Point2, const Vector3& v3Point3)
		: m_v3PositionVector(v3Point1)
	{
		Vector3 v3Vector1(v3Point2 - v3Point1);
		Vector3 v3Vector2(v3Point3 - v3Point1);

		m_v3Normal = v3Vector1.cross(v3Vector2);

		if(m_v3Normal.isZero())
		{
			m_bOk = false;
			return;
		}

		m_bOk = true;

		m_v3Normal.normalize();
	}

	virtual ~Plane() {}

	inline Vector3 GetNormal()
	{
		return m_v3Normal;
	}

	inline void normalize()
	{
		float mag = sqrt(a*a + b*b + c*c);
		mag = 1.0f/mag;
		a *= mag;
		b *= mag;
		c *= mag;
		d *= mag;
	}

	inline float distance(const Vector3& point) const
	{
		return (a*point.x + b*point.y + c * point.z + d);
	}

	inline location_to_plane classifyPoint(const Vector3& point) const
	{
		float td = distance(point);

		if(td < 0.0f) 
			return P_NEGATIVE;
		else 
			return P_POSITIVE;
	}

public:
	bool	m_bOk;
	float	m_fDist;
	
	Vector3 m_v3Normal;
	float a, b, c, d;
	Vector3 m_v3PositionVector;
private:
	
	
};

#endif