/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef RAY_H
#define RAY_H

#include "flx_math.h"
#include "flx_vector3.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
class Ray

{
public:
	inline Ray()
	{
		m_v3Origin		= Vector3(0, 0, 0);
		m_v3Direction	= Vector3(0, 1, 0);
	}

	inline Ray(Vector3 v3Origin, Vector3 v3End)
	{
		m_v3Origin		= v3Origin;
		m_v3End			= v3End;
		m_v3Direction	= (v3End - m_v3Origin);
	}

	inline bool intersectsPoint(Vector3 point, float &dist)
	{
		float	t	= ( dot( (point - m_v3Origin), m_v3Direction ) ) / dot(m_v3Direction, m_v3Direction);
		Vector3 v	= m_v3Origin + t*m_v3Direction;
		float d		= (point - v).magnitude_sq();

		dist = d;
		if(d < 0.1f && d > - 0.1)
			return true;
		else
			return false;
	}

	bool Intersects(const Vector3 vc0, const Vector3 vc1, const Vector3 vc2, bool bCull, float *t) 
	{
		Vector3 pvec, tvec, qvec;
		Vector3 edge1 = vc1 - vc0;
		Vector3 edge2 = vc2 - vc0;
		pvec.cross(this->m_v3Direction, edge2);

		// Wenn nahe 0 ist Strahl parallel
		float det = edge1 * pvec;

		if ( (bCull) && (det < 0.0001f) )
			return false;
		else if ( (det < 0.0001f) && (det > -0.0001f) )
			return false;

		// Entfernung zur Ebene, < 0 = hinter der Ebene
		tvec = this->m_v3Origin - vc0;
		float u = tvec * pvec;

		if (u < 0.0f || u > det)
			return false;

		qvec.cross(tvec, edge1);

		float v = this->m_v3Direction * qvec;

		if (v < 0.0f || u+v > det)
			return false;

		if (t) 
		{
			*t = edge2 * qvec;
			float fInvDet = 1.0f / det;
			*t *= fInvDet;
		}
		return true;
	} // Intersects(Tri)

	virtual ~Ray() {}

public:
	Vector3 m_v3Origin;
	Vector3 m_v3End;
	Vector3 m_v3Direction;
};

#endif