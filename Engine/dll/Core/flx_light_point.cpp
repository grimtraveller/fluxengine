/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_light_point.h"
#include "flx_renderer.h"

LightPoint::LightPoint() : Light()
{
	m_LightType = LIGHT_POINT;
	m_f4Position[3] = 1.0f;

	m_bOn = true;
}

LightPoint::~LightPoint()
{
}

void LightPoint::SetDirection(Vector3 Direction)
{
}

void LightPoint::SetPosition(Vector3 Position)
{
	m_f4Position[0] = Position.x;
	m_f4Position[1] = Position.y;
	m_f4Position[2] = Position.z;
}



void LightPoint::SetAttenuation(Vector3 Attenuation)
{
	if(Attenuation.x < 0.0f) Attenuation.x = 0.0f;
	if(Attenuation.y < 0.0f) Attenuation.y = 0.0f;
	if(Attenuation.z < 0.0f) Attenuation.z = 0.0f;

	m_f3Attenuation[0] = Attenuation.x;
	m_f3Attenuation[1] = Attenuation.y;
	m_f3Attenuation[2] = Attenuation.z;
}

float* LightPoint::GetAttenuation()
{
	return &m_f3Attenuation[0];
}

void LightPoint::Render(int iLightCounter)
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

		glLightf(m_iLightID, GL_CONSTANT_ATTENUATION,	m_f3Attenuation[0]);
		glLightf(m_iLightID, GL_LINEAR_ATTENUATION,		m_f3Attenuation[1]);
		glLightf(m_iLightID, GL_QUADRATIC_ATTENUATION,	m_f3Attenuation[2]);

		break;
	}
}



