/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include "../ObjectLib/flx_vertex3.h"
#include "flx_math.h"

extern class Matrix4x4;

class Vector3
{
public:
	inline Vector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	inline Vector3(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{
	}

	inline Vector3(Vertex3 v)
		: x(v.m_x), y(v.m_y), z(v.m_z)
	{
	}

	inline void set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	virtual ~Vector3() {}

	/*
	inline operator float*() { 
		float f[3];

		f[0] = this->x;
		f[1] = this->y;
		f[2] = this->z;
		return f;
	}

	inline operator const float*() { 
		float f[3];

		f[0] = this->x;
		f[1] = this->y;
		f[2] = this->z;
		return f;
	}
	*/

	inline float magnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}

	inline float magnitude_sq()
	{
		return (x * x + y * y + z * z);
	}

	inline Vector3 project(const Vector3& w, const Vector3& r)
	{
		//      w * r
		// p = ------   * r^2
		//     r*r
		return ( ((w*r)/(r*r)) *r );
	}

	inline Vector3 RotateY(float fRadians)
	{
		static Vector3 v;

		v.x = x*cos(fRadians) - z*sin(fRadians);
		v.y = y;
		v.z = x*sin(fRadians) + z*cos(fRadians);

		return v;
	}

	inline Vector3 RotateX(float fRadians)
	{
		static Vector3 v;

		v.x = x;
		v.y = y*cos(fRadians) + z*sin(fRadians);
		v.z =-y*sin(fRadians) + z*cos(fRadians);

		return v;
	}

	//operator *
	inline Vector3 operator*(const float &fValue) const 
	{
		return Vector3(x * fValue, y * fValue, z * fValue);
	}

	inline friend Vector3 operator*(const float &fValue, const Vector3 v)
	{
		return Vector3(fValue * v.x, fValue * v.y, fValue * v.z);
	}

	inline float operator*(const Vector3 &v2) const
	{
		return x*v2.x + y+v2.y + z+v2.z;
	}

	inline Vector3& operator*=(const float &fValue)
	{
		x *= fValue;
		y *= fValue;
		z *= fValue;

		return *this;
	}

	inline Vector3& operator*=(const Vector3 &v2)
	{
		x *= v2.x;
		y *= v2.y;
		z *= v2.z;

		return *this;
	}

	inline bool operator!=(const Vector3 &v2)
	{
		return (x != v2.x && y != v2.y &&z != v2.z);
	} 

	inline Vector3 operator/(const float &fValue) const 
	{
		return Vector3(x / fValue, y / fValue, z / fValue);
	}

	inline friend Vector3 operator/(const float &fValue, const Vector3 v)
	{
		return Vector3(fValue / v.x, fValue / v.y, fValue / v.z);
	}


	inline Vector3 operator+(const float &fValue) const 
	{
		return Vector3(x + fValue, y + fValue, z + fValue);
	}
	inline Vector3 operator-(const float &fValue) const 
	{
		return Vector3(x - fValue, y - fValue, z - fValue);
	}

	inline Vector3 operator+(const Vector3 &vec3) const
	{
		return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
	}

	//operator +=
	inline Vector3& operator+=(const Vector3 &vec3)
	{
		x += vec3.x;
		y += vec3.y;
		z += vec3.z;

		return *this;
	}

	inline Vector3& operator=(const Vector3 &vec3)
	{
		x = vec3.x;
		y = vec3.y;
		z = vec3.z;

		return *this;
	}



	inline Vector3& operator-=(const Vector3 &vec3)
	{
		x -= vec3.x;
		y -= vec3.y;
		z -= vec3.z;

		return *this;
	}

	inline Vector3 operator-(const Vector3 &v) const
	{
		return Vector3( x - v.x, y - v.y, z - v.z);
	}

	inline Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	inline void SetPerpendicularXZ(Vector3 v)
	{
		x = -v.z;
		y = v.y;
		z = v.x;
	}

	inline Vector3 cross(const Vector3& v) {
		return Vector3
			(
			y * v.z - z * v.y, 
			z * v.x - x * v.z, 
			x * v.y - y * v.x
			);
	}

	inline Vector3 inverse()
	{
		return Vector3(x*-1, y*-1, z*-1);
	}

	inline static Vector3 cross(const Vector3 &v1, const Vector3 &v2) 
	{
		/*
		x = v1.y * v2.z - v1.z * v2.y;
		y = v1.z * v2.x - v1.x * v2.z;
		z = v1.x * v2.y - v1.y * v2.x;
		//w = 1.0f;
		*/

		return Vector3(
			v1.y * v2.z - v1.z * v2.y, 
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	inline static float dot(const Vector3& v1, const Vector3& v2)
	{ 
		return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); 
	}

	inline Vector3 normalize()
	{
		fMag = sqrt(x * x + y * y + z * z);
		if(fMag < ZERO_E5)
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;

			return *this;
		}
		float fMag2 = 1.0f/fMag;

		x *= fMag2;
		y *= fMag2;
		z *= fMag2;

		return *this;
		
	}

	inline Vector3 operator * (const Matrix4x4 &m)
		const {
			Vector3 vcResult;
			/*if (!g_bSSE) {
			vcResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
			vcResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
			vcResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;
			vcResult.w = x*m._14 + y*m._24 + z*m._34 + m._44;
			vcResult.x = vcResult.x/vcResult.w;
			vcResult.y = vcResult.y/vcResult.w;
			vcResult.z = vcResult.z/vcResult.w;
			vcResult.w = 1.0f;
			}
			else {*/
			float *ptrRet = (float*)&vcResult;
			__asm {
				mov ecx, this
					mov edx, m
					movss xmm0, [ecx]
				mov eax, ptrRet
					shufps xmm0, xmm0, 0
					movss xmm1, [ecx+4]
				mulps xmm0, [edx]
				shufps xmm1, xmm1, 0
					movss xmm2, [ecx+8]
				mulps xmm1, [edx+16]
				shufps xmm2, xmm2, 0
					movss xmm3, [ecx+12]
				mulps xmm2, [edx+32]
				shufps xmm3, xmm3, 0
					addps xmm0, xmm1
					mulps xmm3, [edx+48]
				addps xmm2, xmm3
					addps xmm0, xmm2
					movups [eax], xmm0
					mov [eax+3], 1 ; w = 1
			}
			return vcResult;
	}

	inline bool isZero() const 
	{
		if(fabs(x) < ZERO_E5 &&
			fabs(y) < ZERO_E5 &&
			fabs(z) < ZERO_E5)
		{
			return true;
		} else {
			return false;
		}
	}


public:
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float _m[3];
	};
	float w;
	float fMag;
private:
};

inline float dot(const Vector3& v1, const Vector3& v2)
{ return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); }

inline Vector3 cross(const Vector3 &v1, const Vector3 &v2) 
{
		return Vector3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
				);
}

inline Vector3 normalize(const Vector3& v)
{
	Vector3 ret = v;
	float fMag = sqrt(ret.x * ret.x + ret.y * ret.y + ret.z * ret.z);
		if(fMag < ZERO_E5)
		{
			ret.x = 0.0f;
			ret.y = 0.0f;
			ret.z = 0.0f;

			return ret;
		}
		float fMag2 = 1.0f/fMag;

		ret *= fMag2;
		ret *= fMag2;
		ret *= fMag2;

		return ret;
}

#endif