/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef LightDIRECTIONAL_H
#define LightDIRECTIONAL_H

#include "flx_light.h"

class ENGINE_API LightDirectional : public Light
{
public:
	LightDirectional();
	virtual ~LightDirectional();

	void SetPosition(Vector3 Position);
	void SetDirection(Vector3 Direction);
	float* GetDirection();

	void Render(int iLightCounter);

private:
	float m_f4Temp[4];
};


#endif