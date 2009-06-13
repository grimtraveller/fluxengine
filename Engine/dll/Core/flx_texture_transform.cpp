/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_texture_transform.h"

TextureTransform::TextureTransform()
{
	m_f2Scale[0] = 1.0f;
	m_f2Scale[1] = 1.0f;

	m_fRotate = 0.0f;

	m_f2Centre[0] = 0.0f;
	m_f2Centre[1] = 0.0f;

	m_f2Translate[0] = 0.0f;
	m_f2Translate[1] = 0.0f;

}

TextureTransform::~TextureTransform()
{

}