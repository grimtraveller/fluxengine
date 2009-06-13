/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef TEXTURETRANSFORM_H
#define TEXTURETRANSFORM_H

class TextureTransform
{
public:
	TextureTransform();
	virtual ~TextureTransform();

public:
	float m_f2Scale[2];
	float m_fRotate;
	float m_f2Centre[2];
	float m_f2Translate[2];
};

#endif