/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef MATRIX4_H
#define MATRIX4_H

#include <math.h>
#include <string.h>
#include "flx_vector3.h"

class Matrix4x4
{
public:

	union {
			float _11, _21, _31, _41;
			float _12, _22, _32, _42;
			float _13, _23, _33, _43;
			float _14, _24, _34, _44;
		float _m[4][4];
		float m[16];
	};

	Matrix4x4::Matrix4x4() { };

	Matrix4x4::Matrix4x4
		( 
		float __11, float __12, float __13, float __14,
		float __21, float __22, float __23, float __24,
		float __31, float __32, float __33, float __34,
		float __41, float __42, float __43, float __44
		)
	{
		m[0] = __11; m[1] = __12; m[2] = __13; m[3] = __14;
		m[4] = __21; m[5] = __22; m[6] = __23; m[7] = __24;
		m[8] = __31; m[9] = __32; m[10] = __33; m[11] = __34;
		m[12] = __41; m[13] = __42; m[14] = __43; m[15] = __44;
	}

	inline void Matrix4x4::identity(void) 
	{
		float *f = (float*)&this->_11;
		memset(f, 0, sizeof(Matrix4x4));

		_11 = _22 = _33 = _44 = 1.0f;
	}

	inline void Matrix4x4::invert()
	{
		float det = determinant();

		if(!det)
			return;

		Matrix4x4 adjoin = adjoint();
		adjoin.transpose();

		(*this) = adjoin * (1.0f / det);
	}

	inline float Matrix4x4::determinant() const
	{
		return (_11 * minor(1, 2, 3, 1, 2, 3) -
			_21 * minor(0, 2, 3, 1, 2, 3) +
			_31 * minor(0, 1, 3, 1, 2, 3) -
			_41 * minor(0, 1, 2, 1, 2, 3));
	}

	inline float Matrix4x4::minor(unsigned int r0, unsigned int r1, unsigned int r2,
		unsigned int c0, unsigned int c1, unsigned int c2) const
	{
		return (_m[r0][c0] * (_m[r1][c1] * _m[r2][c2] - _m[r2][c1] * _m[r1][c2]) +
			_m[r0][c1] * (_m[r1][c2] * _m[r2][c0] - _m[r1][c0] * _m[r2][c2]) +
			_m[r0][c2] * (_m[r1][c0] * _m[r2][c1] - _m[r1][c1] * _m[r2][c0]));
	}

	inline Matrix4x4 Matrix4x4::adjoint() const
	{
		return Matrix4x4(minor(1, 2, 3, 1, 2, 3),
			-minor(1, 2, 3, 0, 2, 3),
			minor(1, 2, 3, 0, 1, 3),
			-minor(1, 2, 3, 0, 1, 2),

			-minor(0, 2, 3, 1, 2, 3),
			minor(0, 2, 3, 0, 2, 3),
			-minor(0, 2, 3, 0, 1, 3),
			minor(0, 2, 3, 0, 1, 2),

			minor(0, 1, 3, 1, 2, 3),
			-minor(0, 1, 3, 0, 2, 3),
			minor(0, 1, 3, 0, 1, 3),
			-minor(0, 1, 3, 0, 1, 2),

			-minor(0, 1, 2, 1, 2, 3),
			minor(0, 1, 2, 0, 2, 3),
			-minor(0, 1, 2, 0, 1, 3),
			minor(0, 1, 2, 0, 1, 2));
	}

	inline void Matrix4x4::transpose()
	{
		Matrix4x4 mat= (*this);
		mat._m[0][0] = _m[0][0];
		mat._m[0][1] = _m[1][0];
		mat._m[0][2] = _m[2][0];
		mat._m[0][3] = _m[3][0];

		mat._m[1][0] = _m[0][1];
		mat._m[1][1] = _m[1][1];
		mat._m[1][2] = _m[2][1];
		mat._m[1][3] = _m[3][1];

		mat._m[2][0] = _m[0][2];
		mat._m[2][1] = _m[1][2];
		mat._m[2][2] = _m[2][2];
		mat._m[2][3] = _m[3][2];

		mat._m[3][0] = _m[0][3];
		mat._m[3][1] = _m[1][3];
		mat._m[3][2] = _m[2][3];
		mat._m[3][3] = _m[3][3];

		(*this) = mat;
	}

	inline Matrix4x4 Matrix4x4::operator *(const Matrix4x4 &b) const
	{
		const Matrix4x4& a = *this;
		Matrix4x4 mat;

		for (int i=0;i<4;++i) 
		{
			for (int j = 0;j < 4;++j) 
			{
				mat.m[i*4+j] = 0.0;
				for (int k = 0; k < 4; ++k) 
				{
					mat.m[i*4+j] += a.m[i*4+k] * b.m[k*4+j];
				}
			}
		}
		return mat;
	}

	inline Vector3 Matrix4x4::multiply(const Vector3& _vec)
	{
		Vector3 vecResult
			(
			_11 * _vec.x + _12 * _vec.y + _13 * _vec.z,
			_21 * _vec.x + _22 * _vec.y + _23 * _vec.z,
			_31 * _vec.y + _32 * _vec.y + _33 * _vec.z
			);

		return vecResult;
	}


	Matrix4x4 Matrix4x4::operator *(const float &value) const
	{
		const Matrix4x4& a = *this;

		Matrix4x4 c;
		c._11 = a._11 * value;
		c._12 = a._12 * value;
		c._13 = a._13 * value;
		c._14 = a._14 * value;

		c._21 = a._21 * value;
		c._22 = a._22 * value;
		c._23 = a._23 * value;
		c._24 = a._24 * value;

		c._31 = a._31 * value;
		c._32 = a._32 * value;
		c._33 = a._33 * value;
		c._34 = a._34 * value;

		c._41 = a._41 * value;
		c._42 = a._42 * value;
		c._43 = a._43 * value;
		c._44 = a._44 * value;

		return c;
	}


	inline void Matrix4x4::RotaX(float a) 
	{
		float f_cos = cosf(a);
		float f_sin = sinf(a);

		_22 = f_cos;
		_23 = f_sin;
		_32 = -f_sin;
		_33 = f_cos;
		_11 = _44 = 1.0f;
		_12=_13=_14=_21=_24=_31=_34=_41=_42=_43=0.0f;
	}

	inline void Matrix4x4::RotaY(float a) {
		float f_cos = cosf(a);
		float f_sin = sinf(a);
		_11 = f_cos;
		_13 = -f_sin;
		_31 = f_sin;
		_33 = f_cos;
		_22 = _44 = 1.0f;
		_12 = _23 = _14 = _21 = _24 = _32 = _34 = _41 = _42 = _43 = 0.0f;
	}

	inline void Matrix4x4::RotaArbi(Vector3 vcAxis, float a) {
		float fCos = cosf(a);
		float fSin = sinf(a);
		float fSum = 1.0f - fCos;
		vcAxis.normalize();

		_11 = (vcAxis.x*vcAxis.x) * fSum + fCos;
		_12 = (vcAxis.x*vcAxis.y) * fSum - (vcAxis.x*fSin);
		_13 = (vcAxis.x*vcAxis.z) * fSum + (vcAxis.y*fSin);
		_21 = (vcAxis.y*vcAxis.x) * fSum + (vcAxis.z*fSin);
		_22 = (vcAxis.y*vcAxis.y) * fSum + fCos ;
		_23 = (vcAxis.y*vcAxis.z) * fSum - (vcAxis.x*fSin);
		_31 = (vcAxis.z*vcAxis.x) * fSum - (vcAxis.y*fSin);
		_32 = (vcAxis.z*vcAxis.y) * fSum + (vcAxis.x*fSin);
		_33 = (vcAxis.z*vcAxis.z) * fSum + fCos;
		_14 = _24 = _34 = _41 = _42 = _43 = 0.0f;
		_44 = 1.0f;
	}
	inline void Matrix4x4::Translatef(float dx, float dy, float dz) 
	{
		_41 = dx;
		_42 = dy;
		_43 = dz;
		_44 = 1;
	}

	inline void Matrix4x4::toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const
	{
		// Extracts the Euler angles from a rotation matrix. The returned
		// angles are in degrees. This method might suffer from numerical
		// imprecision for ill defined rotation matrices.
		//
		// This function only works for rotation matrices constructed using
		// the popular NASA standard airplane convention of heading-pitch-roll 
		// (i.e., RzRxRy).
		//
		// The algorithm I use here is from a paper written by David Eberly
		// titled "Euler Angle Formulas". This paper can be found on his
		// Magic Software web site (http://www.magic-software.com).

		float thetaX = asinf(mtx[1][2]);
		float thetaY = 0.0f;
		float thetaZ = 0.0f;

		if (thetaX < Math::HALF_PI)
		{
			if (thetaX > -Math::HALF_PI)
			{
				thetaZ = atan2f(-mtx[1][0], mtx[1][1]);
				thetaY = atan2f(-mtx[0][2], mtx[2][2]);
			}
			else
			{
				// Not a unique solution.
				thetaZ = -atan2f(mtx[2][0], mtx[0][0]);
				thetaY = 0.0f;
			}
		}
		else
		{
			// Not a unique solution.
			thetaZ = atan2f(mtx[2][0], mtx[0][0]);
			thetaY = 0.0f;
		}

		headDegrees = Math::radiansToDegrees(thetaY);
		pitchDegrees = Math::radiansToDegrees(thetaX);
		rollDegrees = Math::radiansToDegrees(thetaZ);
	}


	inline void Matrix4x4::fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
	{
		// Constructs a rotation matrix based on a Euler Transform.
		// I use the popular NASA standard airplane convention of 
		// heading-pitch-roll (i.e., RzRxRy).

		headDegrees = Math::degreesToRadians(headDegrees);
		pitchDegrees = Math::degreesToRadians(pitchDegrees);
		rollDegrees = Math::degreesToRadians(rollDegrees);

		float cosH = cosf(headDegrees);
		float cosP = cosf(pitchDegrees);
		float cosR = cosf(rollDegrees);
		float sinH = sinf(headDegrees);
		float sinP = sinf(pitchDegrees);
		float sinR = sinf(rollDegrees);

		mtx[0][0] = cosR * cosH - sinR * sinP * sinH;
		mtx[0][1] = sinR * cosH + cosR * sinP * sinH;
		mtx[0][2] = -cosP * sinH;
		mtx[0][3] = 0.0f;

		mtx[1][0] = -sinR * cosP;
		mtx[1][1] = cosR * cosP;
		mtx[1][2] = sinP;
		mtx[1][3] = 0.0f;

		mtx[2][0] = cosR * sinH + sinR * sinP * cosH;
		mtx[2][1] = sinR * sinH - cosR * sinP * cosH;
		mtx[2][2] = cosP * cosH;
		mtx[2][3] = 0.0f;

		mtx[3][0] = 0.0f;
		mtx[3][1] = 0.0f;
		mtx[3][2] = 0.0f;
		mtx[3][3] = 1.0f;
	}

/*
	inline float* Matrix4x4::GetFloatv()
	{
		float f[16];
		f[0] = _11;
		f[1] = _12;
		f[2] = _13;
		f[3] = _14;

		f[4] = _21;
		f[5] = _22;
		f[6] = _23;
		f[7] = _24;

		f[8] = _31;
		f[9] = _32;
		f[10] = _33;
		f[11] = _34;

		f[12] = _41;
		f[13] = _42;
		f[14] = _43;
		f[15] = _44;

		return f;
	}
*/
	float mtx[4][4];
};

#endif