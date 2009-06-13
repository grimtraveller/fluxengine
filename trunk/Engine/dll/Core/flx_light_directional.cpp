/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_light_directional.h"
#include "flx_renderer.h"

LightDirectional::LightDirectional() : Light()
{
	m_LightType = LIGHT_DIRECTIONAL;
	m_f4Position[3] = 0.0f;

	m_bOn = true;
}

LightDirectional::~LightDirectional()
{
}

void LightDirectional::SetPosition(Vector3 Position)
{
}

void LightDirectional::SetDirection(Vector3 Direction)
{
	m_f4Position[0] = -Direction.x;
	m_f4Position[1] = -Direction.y;
	m_f4Position[2] = -Direction.z;
}

float* LightDirectional::GetDirection()
{
	m_f4Temp[0] = -m_f4Position[0];
	m_f4Temp[1] = -m_f4Position[1];
	m_f4Temp[2] = -m_f4Position[2];
	m_f4Temp[3] = -m_f4Position[3];
	return &m_f4Temp[0];
}

void LightDirectional::Render(int iLightCounter)
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

		break;
	}
}


