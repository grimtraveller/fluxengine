/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_light_spot.h"
#include "flx_renderer.h"


LightSpot::LightSpot() : Light()
{
	m_LightType = LIGHT_SPOT;
	m_f4Position[3] = 1.0f;

	m_fCutoff	= 45.0f;
	m_fExponent = 50.0f;

	m_bOn = true;
}

LightSpot::~LightSpot()
{
}

void LightSpot::SetPosition(Vector3 Position)
{
	m_f4Position[0] = Position.x;
	m_f4Position[1] = Position.y;
	m_f4Position[2] = Position.z;
}

float* LightSpot::GetPosition()
{
	return &m_f4Position[0];
}

void LightSpot::SetDirection(Vector3 Direction)
{
	m_f3Direction[0] = Direction.x;
	m_f3Direction[1] = Direction.y;
	m_f3Direction[2] = Direction.z;
}

float* LightSpot::GetDirection()
{
	return &m_f3Direction[0];
}

void LightSpot::SetAttenuation(Vector3 Attenuation)
{
	if(Attenuation.x < 0.0f) Attenuation.x = 0.0f;
	if(Attenuation.y < 0.0f) Attenuation.y = 0.0f;
	if(Attenuation.z < 0.0f) Attenuation.z = 0.0f;

	m_f3Attenuation[0] = Attenuation.x;
	m_f3Attenuation[1] = Attenuation.y;
	m_f3Attenuation[2] = Attenuation.z;
}

float* LightSpot::GetAttenuation()
{
	return &m_f3Attenuation[0];
}

void LightSpot::SetExponent(float fExponent)
{
	if(fExponent <= 0) 
		fExponent = 0;
	else if(fExponent >= 128.0f)
		fExponent = 128.0f;

	m_fExponent = fExponent;
}

float LightSpot::GetExponent()
{
	return m_fExponent;
}


void LightSpot::SetCutoff(float fCutoff)
{
	if(fCutoff <= 0) 
		fCutoff = 0;
	else if(fCutoff >= 90.0f)
		fCutoff = 90.0f;

	m_fCutoff = fCutoff;
}

float LightSpot::GetCutoff()
{
	return m_fCutoff;
}

void LightSpot::Render(int iLightCounter)
{
	switch (Flux::Renderer::getInstance()->GetRendererType())
	{
	case R_OPENGL:
		m_iLightID = GL_LIGHT0 + iLightCounter;
		glEnable(m_iLightID);

		glLightfv(m_iLightID, GL_AMBIENT, m_f4ColorAmbient);
		glLightfv(m_iLightID, GL_DIFFUSE, m_f4ColorDiffuse);
		glLightfv(m_iLightID, GL_SPECULAR, m_f4ColorSpecular);

		glLightfv(m_iLightID, GL_POSITION, m_f4Position);

		glLightfv(m_iLightID, GL_SPOT_DIRECTION, m_f3Direction);
		glLightf(m_iLightID, GL_SPOT_EXPONENT, m_fExponent);
		glLightf(m_iLightID, GL_SPOT_CUTOFF, m_fCutoff);

		glLightf(m_iLightID, GL_CONSTANT_ATTENUATION,	m_f3Attenuation[0]);
		glLightf(m_iLightID, GL_LINEAR_ATTENUATION,		m_f3Attenuation[1]);
		glLightf(m_iLightID, GL_QUADRATIC_ATTENUATION,	m_f3Attenuation[2]);

		break;
	}
}



