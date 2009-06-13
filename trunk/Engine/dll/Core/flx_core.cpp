/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_core.h"
#include <math.h>
#include "flx_logger.h"

#include <list>
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))


AABBox::AABBox()
{}

AABBox::AABBox(Vector3& _max, Vector3& _min) :
vecMax(_max), vecMin(_min)
{
}

void AABBox::getCorners(Vector3 pt[]) const
{
	pt[0] = Vector3(vecMin.x, vecMin.y, vecMin.z);
	pt[1] = Vector3(vecMax.x, vecMin.y, vecMin.z);
	pt[2] = Vector3(vecMin.x, vecMax.y, vecMin.z);
	pt[3] = Vector3(vecMin.x, vecMin.y, vecMax.z);
	pt[4] = Vector3(vecMax.x, vecMax.y, vecMin.z);
	pt[5] = Vector3(vecMin.x, vecMax.y, vecMax.z);
	pt[6] = Vector3(vecMax.x, vecMin.y, vecMax.z);
	pt[7] = Vector3(vecMax.x, vecMax.y, vecMax.z);
}

void AABBox::render()
{
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex3f(vecMin.x, vecMin.y, vecMin.z); //pt0
	glVertex3f(vecMax.x, vecMin.y, vecMin.z); //pt1

	glVertex3f(vecMax.x, vecMin.y, vecMin.z); //pt1
	glVertex3f(vecMax.x, vecMin.y, vecMax.z); //pt2

	glVertex3f(vecMax.x, vecMin.y, vecMax.z); //pt2
	glVertex3f(vecMin.x, vecMin.y, vecMax.z); //pt3

	glVertex3f(vecMin.x, vecMin.y, vecMax.z); //pt3
	glVertex3f(vecMin.x, vecMin.y, vecMin.z); //pt0

	glVertex3f(vecMin.x, vecMax.y, vecMin.z); //pt4
	glVertex3f(vecMax.x, vecMax.y, vecMin.z); //pt5

	glVertex3f(vecMax.x, vecMax.y, vecMin.z); //pt5
	glVertex3f(vecMax.x, vecMax.y, vecMax.z); //pt6

	glVertex3f(vecMax.x, vecMax.y, vecMax.z); //pt6
	glVertex3f(vecMin.x, vecMax.y, vecMax.z); //pt7

	glVertex3f(vecMin.x, vecMax.y, vecMax.z); //pt7
	glVertex3f(vecMin.x, vecMax.y, vecMin.z); //pt4

	glVertex3f(vecMin.x, vecMin.y, vecMin.z); //pt0
	glVertex3f(vecMin.x, vecMax.y, vecMin.z); //pt4

	glVertex3f(vecMax.x, vecMin.y, vecMin.z); //pt1
	glVertex3f(vecMax.x, vecMax.y, vecMin.z); //pt5

	glVertex3f(vecMax.x, vecMin.y, vecMax.z); //pt2
	glVertex3f(vecMax.x, vecMax.y, vecMax.z); //pt6

	glVertex3f(vecMin.x, vecMin.y, vecMax.z); //pt3
	glVertex3f(vecMin.x, vecMax.y, vecMax.z); //pt7
	glEnd();
}

bool AABBox::intersectionRay(Ray ray)
{

	// Compute the ray delta
	Vector3 rayDelta = ray.m_v3Direction; //;* ray.m_v3Direction.magnitude();

	// first we check to see if the origin of the ray is 
	// inside the AABB.  If it is, the ray intersects the AABB so 
	// we'll return true.  We start by assuming the ray origin is 
	// inside the AABB
	bool inside = true;

	// this stores the distance from either the min or max (x,y,z) to the ray's
	// origin (x,y,z) respectively, divided by the length of the ray.  the largest
	// value has the delta time of a possible intersection.
	float xt, yt, zt;
	
	// Test the X component of the ray's origin to see if we are inside or not
	if(ray.m_v3Origin.x < vecMin.x)
	{
		xt = vecMin.x - ray.m_v3Origin.x;
		
		if(xt > rayDelta.x) // if the ray is moving away from the AABB, there is no intersection 
			return false;
		
		xt /= rayDelta.x; 
		inside = false;
	} 
	else if(ray.m_v3Origin.x > vecMax.x)
	{
		xt = vecMax.x - ray.m_v3Origin.x;
		
		if(xt < rayDelta.x) // if the ray is moving away from the AABB, there is no intersection 
			return false;
		
		xt /= rayDelta.x;
		inside = false;
	} 
	else
	{
		xt = -1.0f; 
	}

	// test the X component of the ray's origin to see if we are inside or not
	if(ray.m_v3Origin.y < vecMin.y)
	{
		yt = vecMin.y - ray.m_v3Origin.y;
		
		if(yt > rayDelta.y) // if the ray is moving away from the AABB, there is no intersection 
			return false;

		yt /= rayDelta.y;
		inside = false;
	} 
	else if(ray.m_v3Origin.y > vecMax.y)
	{
		yt = vecMax.y - ray.m_v3Origin.y;
		
		if(yt < rayDelta.y) // if the ray is moving away from the AABB, there is no intersection 
			return false;
		
		yt /= rayDelta.y;
		inside = false;
	} 
	else
	{
		yt = -1.0f;
	}

	if(ray.m_v3Origin.z < vecMin.z)
	{
		zt = vecMin.z - ray.m_v3Origin.z;
		
		if(zt > rayDelta.z) // if the ray is moving away from the AABB, there is no intersection 
			return false;
		
		zt /= rayDelta.z;
		inside = false;
	} 
	else if(ray.m_v3Origin.z > vecMax.z)
	{
		zt = vecMax.z - ray.m_v3Origin.z;
		
		if(zt < rayDelta.z) // if the ray is moving away from the AABB, there is no intersection 
			return false;
		
		zt /= rayDelta.z;
		inside = false;
	} 
	else
	{
		zt = -1.0f;
	}

	if(inside)
		return true;

	float t = xt;
	
	if(yt > t)
		t = yt;
	
	if(zt > t)
		t = zt;

	if(t == xt)
	{
		// compute intersection values
		float y = ray.m_v3Origin.y + rayDelta.y * t;
		float z = ray.m_v3Origin.z + rayDelta.z * t;
	
		// test to see if collision takes place within the bounds of the AABB
		if(y < vecMin.y || y > vecMax.y)
			return false;
		else if(z < vecMin.z || z > vecMax.z)
			return false;
	}
	else if(t == yt) // intersects with the XZ plane
	{
		// Compute intersection values
		float x = ray.m_v3Origin.x + rayDelta.x * t;
		float z = ray.m_v3Origin.z + rayDelta.z * t;

		// test to see if collision takes place within the bounds of the AABB
		if(x < vecMin.x || x > vecMax.x)
			return false;
		else if(z < vecMin.z || z > vecMax.z) 
			return false;
	}
	else // intersects with the XY plane
	{
		assert(t == zt);

		// vompute intersection values
		float x = ray.m_v3Origin.x + rayDelta.x * t;
		float y = ray.m_v3Origin.y + rayDelta.y * t;

		// test to see if collision takes place within the bounds of the AABB
		if(x < vecMin.x || x > vecMax.x)
			return false;
		else if(y < vecMin.y || y > vecMax.y)
			return false;
	}

	return true;
}

bool AABBox::ClipSegment(float _min, float _max, float a, float b, float d, float& t0, float& t1)
{
	const float threshold = 1.0e-6f;

	if (abs(d) < threshold)
	{
		if (d > 0.0f)
		{
			return !(b < _min || a > _max);
		}
		else
		{
			return !(a < _min || b > _max);
		}
	}

	float u0, u1;

	u0 = (_min - a) / (d);
	u1 = (_max - a) / (d);

	if (u0 > u1) 
	{
		float temp = u0;
		u0 = u1;
		u1 = temp;
	}

	if (u1 < t0 || u0 > t1)
	{
		return false;
	}

	t0 = max(u0, t0);
	t1 = min(u1, t1);

	if (t1 < t0)
	{
		return false;
	}

	return true; 
}


bool AABBox::ClipSegment(Vector3& A, Vector3& B)
{
	Vector3 Min, Max;

	Max = vecMax;
	Min = vecMin;

	Vector3 S = A;
	Vector3 D = (B - A);

	float t0 = 0.0f, t1 = 1.0f;

	if (!ClipSegment(Min.x, Max.x, A.x, B.x, D.x, t0, t1)) 
	{
		return false;
	}
	
	if (!ClipSegment(Min.y, Max.y, A.y, B.y, D.y, t0, t1)) 
	{
		return false;
	}

	if (!ClipSegment(Min.z, Max.z, A.z, B.z, D.z, t0, t1)) 
	{
		return false;
	}

	A = S + D * t0;
	B = S + D * t1;

	return true;
}

AABBox AABBox::transform(const AABBox &box, Matrix4x4 &mat)
{
	float av, bv;
	int   i, j;
	AABBox new_box;
	new_box.vecMax = Vector3(mat.m[12], mat.m[13], mat.m[14]);
	new_box.vecMin = Vector3(mat.m[12], mat.m[13], mat.m[14]);



	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			av = mat._m[i][j] * box.vecMin._m[j];
			bv = mat._m[i][j] * box.vecMax._m[j];
			if (av < bv)
			{
				new_box.vecMin._m[i] += av;
				new_box.vecMax._m[i] += bv;
			} else {
				new_box.vecMin._m[i] += bv;
				new_box.vecMax._m[i] += av;
			}
		}
	return new_box;		
}

bool AABBox::isInBox(const Vector3& _point)
{
	 return (_point.x >= vecMin.x && _point.x <= vecMax.x) &&
           (_point.y >= vecMin.y && _point.y <= vecMax.y) &&
		   (_point.z >= vecMin.z && _point.z <= vecMax.z);
}