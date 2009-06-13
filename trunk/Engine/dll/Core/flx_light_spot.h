/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef LightSPOT_H
#define LightSPOT_H

#include "flx_light.h"

class ENGINE_API LightSpot : public Light
{
public:

	LightSpot();
	virtual ~LightSpot();

	void SetPosition(Vector3 Position);
	float* GetPosition();
	void SetDirection(Vector3 Direction);
	float* GetDirection();

	void SetAttenuation(Vector3 Attenuation);
	float* GetAttenuation();

	void SetExponent(float fExponent);
	float GetExponent();
	void SetCutoff(float fCutoff);
	float GetCutoff();

	void Render(int iLightCounter);
};
	

#endif