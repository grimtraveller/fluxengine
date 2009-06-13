/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_light.h"
#include "flx_renderer.h"

int Light::s_iMaxNumLights = 0;

Light::Light()
{
	m_LightType		= LIGHT_POINT;
	m_bOn			= true;

	m_f4ColorAmbient[0] = 0.0f;	//black ambient light
	m_f4ColorAmbient[1] = 0.0f;
	m_f4ColorAmbient[2] = 0.0f;
	m_f4ColorAmbient[3] = 1.0f;

	m_f4ColorDiffuse[0] = 1.0f;	//white diffuse color
	m_f4ColorDiffuse[1] = 1.0f;
	m_f4ColorDiffuse[2] = 1.0f;
	m_f4ColorDiffuse[3] = 1.0f;  //alpha = 1 -> visible

	m_f4ColorSpecular[0] = 1.0f;
	m_f4ColorSpecular[0] = 1.0f;
	m_f4ColorSpecular[0] = 1.0f;
	m_f4ColorSpecular[0] = 1.0f;

	m_fExponent = 0.0f;
	m_fCutoff	= 180.f;

	m_f3Attenuation[0] = 1.0f;
	m_f3Attenuation[1] = 0.0f;
	m_f3Attenuation[2] = 0.0f;

	m_f4Position[0] = 0.0f;
	m_f4Position[1] = 0.0f;
	m_f4Position[2] = 0.0f;
	m_f4Position[3] = 1.0f;

	m_f3Direction[0] = 0.0f;
	m_f3Direction[1] = 0.0f;
	m_f3Direction[2] = -1.0f;

	switch (Flux::Renderer::getInstance()->GetRendererType())
	{
	case R_OPENGL:
		{
			//how many lights do the gpu support?
			glGetIntegerv(GL_MAX_LIGHTS, &s_iMaxNumLights);

			break;
		}
	}
}

Light::~Light()
{
	//penis :D
}

void Light::Disable(int iLightCounter)
{
	switch (Flux::Renderer::getInstance()->GetRendererType())
	{
	case R_OPENGL:
		m_iLightID = GL_LIGHT0 + iLightCounter;
		glDisable(m_iLightID);

		break;
	}
}

void Light::SetOn(bool bOn)
{
	m_bOn = bOn;
}

bool Light::isOn()
{
	return m_bOn;
}

void Light::SetAmbientColor(ColorOGL fColor)
{
	m_f4ColorAmbient[0] = fColor.m_fRed;
	m_f4ColorAmbient[1] = fColor.m_fGreen;
	m_f4ColorAmbient[2] = fColor.m_fBlue;
	m_f4ColorAmbient[3] = fColor.m_fAlpha;
}

float* Light::GetAmbienColor()
{
	return &m_f4ColorAmbient[0];
}

void Light::SetDiffuseColor(ColorOGL fColor)
{
	m_f4ColorDiffuse[0] = fColor.m_fRed;
	m_f4ColorDiffuse[1] = fColor.m_fGreen;
	m_f4ColorDiffuse[2] = fColor.m_fBlue;
	m_f4ColorDiffuse[3] = fColor.m_fAlpha;
}

float* Light::GetDiffuseColor()
{
	return &m_f4ColorDiffuse[0];
}

void Light::SetSpecularColor(ColorOGL fColor)
{
	m_f4ColorSpecular[0] = fColor.m_fRed;
	m_f4ColorSpecular[1] = fColor.m_fGreen;
	m_f4ColorSpecular[2] = fColor.m_fBlue;
	m_f4ColorSpecular[3] = fColor.m_fAlpha;
}

float* Light::GetSpecularColor()
{
	return &m_f4ColorSpecular[0];
}





