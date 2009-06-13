/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_material.h"

Material::Material()
{
	m_f4Ambient[0] = 0.2f;
	m_f4Ambient[1] = 0.2f;
	m_f4Ambient[2] = 0.2f;
	m_f4Ambient[3] = 0.5f;

	m_f4Diffuse[0] = 0.8f;
	m_f4Diffuse[1] = 0.8f;
	m_f4Diffuse[2] = 0.8f;
	m_f4Diffuse[3] = 0.5f;

	m_f4Specular[0] = 0.0f;
	m_f4Specular[1] = 0.0f;
	m_f4Specular[2] = 0.0f;
	m_f4Specular[3] = 1.0f;

	m_f4Emmissive[0] = 0.0f;
	m_f4Emmissive[1] = 0.0f;
	m_f4Emmissive[2] = 0.0f;
	m_f4Emmissive[3] = 1.0f;

	m_fShininess	= 0;
}

Material::~Material()
{
}