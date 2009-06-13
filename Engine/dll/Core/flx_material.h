/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "flx_core.h"


class ENGINE_API Material
{
public:
	Material();
	virtual ~Material();

public:
	float m_f4Ambient[4];
	float m_f4Diffuse[4];
	float m_f4Specular[4];
	float m_f4Emmissive[4];
	float m_fShininess;
};
#endif