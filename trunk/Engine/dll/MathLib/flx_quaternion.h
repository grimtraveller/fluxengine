/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "flx_vector3.h"
#include "flx_matrix4.h"

class Quaternion
{
	friend Quaternion operator*(float lhs, const Quaternion &rhs);

public:
	static const Quaternion IDENTITY;

	float w, x, y, z;

	Quaternion() {}
	Quaternion(float w_, float x_, float y_, float z_);
	~Quaternion() {}

	bool operator==(const Quaternion &rhs) const;
	bool operator!=(const Quaternion &rhs) const;

	Quaternion &operator+=(const Quaternion &rhs);
	Quaternion &operator-=(const Quaternion &rhs);
	Quaternion &operator*=(const Quaternion &rhs);
	Quaternion &operator*=(float scalar);
	Quaternion &operator/=(float scalar);

	Quaternion operator+(const Quaternion &rhs) const;
	Quaternion operator-(const Quaternion &rhs) const;
	Quaternion operator*(const Quaternion &rhs) const;
	Quaternion operator*(float scalar) const;
	Quaternion operator/(float scalar) const;

	Quaternion conjugate() const;
	void fromAxisAngle(const Vector3 &axis, float degrees);
	void fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
	void fromMatrix(const Matrix4x4 &m);
	void identity();
	Quaternion inverse() const;
	float magnitude() const;
	void normalize();
	void set(float w_, float x_, float y_, float z_);
	void toAxisAngle(Vector3 &axis, float &degrees) const;
	void toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const;
	Matrix4x4 toMatrix4x4() const;
};

inline Quaternion operator*(float lhs, const Quaternion &rhs)
{
	return rhs * lhs;
}

inline Quaternion::Quaternion(float w_, float x_, float y_, float z_)
: w(w_), x(x_), y(y_), z(z_) {}

inline bool Quaternion::operator==(const Quaternion &rhs) const
{
	return Math::closeEnough(w, rhs.w) && Math::closeEnough(x, rhs.x)
		&& Math::closeEnough(y, rhs.y) && Math::closeEnough(z, rhs.z);
}

inline bool Quaternion::operator!=(const Quaternion &rhs) const
{
	return !(*this == rhs);
}

inline Quaternion &Quaternion::operator+=(const Quaternion &rhs)
{
	w += rhs.w, x += rhs.x, y += rhs.y, z += rhs.z;
	return *this;
}

inline Quaternion &Quaternion::operator-=(const Quaternion &rhs)
{
	w -= rhs.w, x -= rhs.x, y -= rhs.y, z -= rhs.z;
	return *this;
}

inline Quaternion &Quaternion::operator*=(const Quaternion &rhs)
{
	// Multiply so that rotations are applied in a left to right order.
	Quaternion tmp(
		(w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
		(w * rhs.x) + (x * rhs.w) - (y * rhs.z) + (z * rhs.y),
		(w * rhs.y) + (x * rhs.z) + (y * rhs.w) - (z * rhs.x),
		(w * rhs.z) - (x * rhs.y) + (y * rhs.x) + (z * rhs.w));

	*this = tmp;
	return *this;
}

inline Quaternion &Quaternion::operator*=(float scalar)
{
	w *= scalar, x *= scalar, y *= scalar, z *= scalar;
	return *this;
}

inline Quaternion &Quaternion::operator/=(float scalar)
{
	w /= scalar, x /= scalar, y /= scalar, z /= scalar;
	return *this;
}

inline Quaternion Quaternion::operator+(const Quaternion &rhs) const
{
	Quaternion tmp(*this);
	tmp += rhs;
	return tmp;
}

inline Quaternion Quaternion::operator-(const Quaternion &rhs) const
{
	Quaternion tmp(*this);
	tmp -= rhs;
	return tmp;
}

inline Quaternion Quaternion::operator*(const Quaternion &rhs) const
{
	Quaternion tmp(*this);
	tmp *= rhs;
	return tmp;
}

inline Quaternion Quaternion::operator*(float scalar) const
{
	Quaternion tmp(*this);
	tmp *= scalar;
	return tmp;
}

inline Quaternion Quaternion::operator/(float scalar) const
{
	Quaternion tmp(*this);
	tmp /= scalar;
	return tmp;
}

inline Quaternion Quaternion::conjugate() const
{
	Quaternion tmp(w, -x, -y, -z);
	return tmp;
}

inline void Quaternion::fromAxisAngle(const Vector3 &axis, float degrees)
{
	float halfTheta = Math::degreesToRadians(degrees) * 0.5f;
	float s = sinf(halfTheta);
	w = cosf(halfTheta), x = axis.x * s, y = axis.y * s, z = axis.z * s;
}

inline void Quaternion::fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
{
	Matrix4x4 m;
	m.fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
	fromMatrix(m);
}

inline void Quaternion::identity()
{
	w = 1.0f, x = y = z = 0.0f;
}

inline Quaternion Quaternion::inverse() const
{
	float invMag = 1.0f / magnitude();
	return conjugate() * invMag;
}

inline float Quaternion::magnitude() const
{
	return sqrtf(w * w + x * x + y * y + z * z);
}

inline void Quaternion::normalize()
{
	float invMag = 1.0f / magnitude();
	w *= invMag, x *= invMag, y *= invMag, z *= invMag;
}

inline void Quaternion::set(float w_, float x_, float y_, float z_)
{
	w = w_, x = x_, y = y_, z = z_;
}

inline void Quaternion::toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const
{
	Matrix4x4 m = toMatrix4x4();
	m.toHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
}


#endif