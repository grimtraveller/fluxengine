/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_vertex3.h"

Vertex3::Vertex3()
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

Vertex3::Vertex3(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

Vertex3 Vertex3::operator-(const Vertex3& v) const 
{
	return Vertex3(m_x-v.m_x, m_y-v.m_y, m_z-v.m_z);
}

Vertex3::~Vertex3()
{

	//lawl nothing! :s
}