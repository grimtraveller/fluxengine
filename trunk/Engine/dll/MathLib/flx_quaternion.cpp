/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_quaternion.h"

const Quaternion Quaternion::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);

void Quaternion::fromMatrix(const Matrix4x4 &m)
{
    float s = 0.0f;
    float q[4] = {0.0f};
    float trace = m._m[0][0] + m._m[1][1] + m._m[2][2];

    if (trace > 0.0f)
    {
        s = sqrtf(trace + 1.0f);
        q[3] = s * 0.5f;
        s = 0.5f / s;
        q[0] = (m._m[1][2] - m._m[2][1]) * s;
        q[1] = (m._m[2][0] - m._m[0][2]) * s;
        q[2] = (m._m[0][1] - m._m[1][0]) * s;
    }
    else
    {
        int nxt[3] = {1, 2, 0};
        int i = 0, j = 0, k = 0;

        if (m._m[1][1] > m._m[0][0])
            i = 1;

        if (m._m[2][2] > m._m[i][i])
            i = 2;

        j = nxt[i];
        k = nxt[j];
        s = sqrtf((m._m[i][i] - (m._m[j][j] + m._m[k][k])) + 1.0f);

        q[i] = s * 0.5f;
        s = 0.5f / s;
        q[3] = (m._m[j][k] - m._m[k][j]) * s;
        q[j] = (m._m[i][j] + m._m[j][i]) * s;
        q[k] = (m._m[i][k] + m._m[k][i]) * s;
    }

    x = q[0], y = q[1], z = q[2], w = q[3];
}

void Quaternion::toAxisAngle(Vector3 &axis, float &degrees) const
{
    // Converts this quaternion to an axis and an angle.

    float sinHalfThetaSq = 1.0f - w * w;

    // Guard against numerical imprecision and identity quaternions.
    if (sinHalfThetaSq <= 0.0f)
    {
        axis.x = 1.0f, axis.y = axis.z = 0.0f;
        degrees = 0.0f;
    }
    else
    {
        float invSinHalfTheta = 1.0f / sqrtf(sinHalfThetaSq);

        axis.x = x * invSinHalfTheta;
        axis.y = y * invSinHalfTheta;
        axis.z = z * invSinHalfTheta;
        degrees = Math::radiansToDegrees(2.0f * acosf(w));
    }
}

Matrix4x4 Quaternion::toMatrix4x4() const
{
    // Converts this quaternion to a rotation matrix.
    //
    //  | 1 - 2(y^2 + z^2)	2(xy + wz)			2(xz - wy)			0  |
    //  | 2(xy - wz)		1 - 2(x^2 + z^2)	2(yz + wx)			0  |
    //  | 2(xz + wy)		2(yz - wx)			1 - 2(x^2 + y^2)	0  |
    //  | 0					0					0					1  |

    float x2 = x + x; 
    float y2 = y + y; 
    float z2 = z + z;
    float xx = x * x2;
    float xy = x * y2;
    float xz = x * z2;
    float yy = y * y2;
    float yz = y * z2;
    float zz = z * z2;
    float wx = w * x2;
    float wy = w * y2;
    float wz = w * z2;

    Matrix4x4 m;

    m._m[0][0] = 1.0f - (yy + zz);
    m._m[0][1] = xy + wz;
    m._m[0][2] = xz - wy;
    m._m[0][3] = 0.0f;

    m._m[1][0] = xy - wz;
    m._m[1][1] = 1.0f - (xx + zz);
    m._m[1][2] = yz + wx;
    m._m[1][3] = 0.0f;

    m._m[2][0] = xz + wy;
    m._m[2][1] = yz - wx;
    m._m[2][2] = 1.0f - (xx + yy);
    m._m[2][3] = 0.0f;

    m._m[3][0] = 0.0f;
    m._m[3][1] = 0.0f;
    m._m[3][2] = 0.0f;
    m._m[3][3] = 1.0f;

    return m;
}