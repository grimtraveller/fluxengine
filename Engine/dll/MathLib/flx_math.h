/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef MATH_H
#define MATH_H


#include <math.h>

const float ZERO_E5 = (float)1e-5;

class Math
{
public:
    static const float PI;
    static const float HALF_PI;
    static const float EPSILON;

    static bool closeEnough(float f1, float f2)
    {
        return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
    }

    static float degreesToRadians(float degrees)
    {
        return (degrees * PI) / 180.0f;
    }

    static float radiansToDegrees(float radians)
    {
        return (radians * 180.0f) / PI;
    }
};


#endif