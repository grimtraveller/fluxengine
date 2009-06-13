/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef Vertex3_H
#define Vertex3_H

class Vertex3
{
public:
	Vertex3();
	Vertex3(float x, float y, float z);
	virtual ~Vertex3();

	Vertex3 operator-(const Vertex3& v) const;

public:
	float m_x;
	float m_y;
	float m_z;
};

#endif