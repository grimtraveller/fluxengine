/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef LightPOINT_H
#define LightPOINT_H

#include "flx_light.h"

class ENGINE_API LightPoint : public Light
{
public:
	LightPoint();
	virtual ~LightPoint();

	void SetPosition(Vector3 Position);

	void SetDirection(Vector3 Direction);

	void SetAttenuation(Vector3 Attenuation);
	float* GetAttenuation();

	void Render(int iLightCounter);
};
	

#endif