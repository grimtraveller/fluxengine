/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef Light_H
#define Light_H

#include "../Core/flx_core.h"
#include "flx_color_ogl.h"
#include "../MathLib/flx_vector3.h"


enum eLightType
{
	LIGHT_POINT = 0,
	LIGHT_DIRECTIONAL,
	LIGHT_SPOT
};

class ENGINE_API Light
{
public:
	Light();
	virtual ~Light();

	void Disable(int iLightCounter);

	void SetOn(bool bOn);
	bool isOn();

	virtual void SetPosition(Vector3 Position) = 0;
	virtual void SetDirection(Vector3 Direction) = 0;

	Vector3 GetPosition() { return Vector3(m_f4Position[0], m_f4Position[1], m_f4Position[2]); }

	void SetAmbientColor(ColorOGL fColor);
	float* GetAmbienColor();
	void SetDiffuseColor(ColorOGL fColor);
	float* GetDiffuseColor();
	void SetSpecularColor(ColorOGL fColor);
	float* GetSpecularColor();

	virtual void Render(int iLightCounter) = 0;

public:
	static int s_iMaxNumLights;

protected:
	eLightType m_LightType;

	bool m_bOn;
	int m_iLightID;

	float m_f4ColorAmbient[4];
	float m_f4ColorDiffuse[4];
	float m_f4ColorSpecular[4];

	float m_fExponent;
	float m_fCutoff;
	float m_f3Attenuation[3];

	float m_f4Position[4];
	float m_f3Direction[3];
};

#endif