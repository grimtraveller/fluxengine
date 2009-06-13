/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_node_transform.h"
#include "flx_scene.h"

NodeTransform::NodeTransform()
{
	m_Type = TRANSFORM;

	m_v3Translate = Vector3(0,0,0);
	m_v3Centre = Vector3(0,0,0);
	m_v3Scale		= Vector3(1.0f, 1.0f, 1.0f);
	//m_v3RotateAxis	= Vector3(0, 0, 1.0f);
	//m_v3RotateAngle	= Vector3(0.0f, 0.0f, 0.0f);
}

NodeTransform::~NodeTransform()
{
}

void NodeTransform::Update()
{
}

//move model view matrix M = (T C R S C^)

void NodeTransform::RenderOGL(bool bRenderSolid)
{
	if(m_pIn && m_bVisible)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_v3Translate.x, m_v3Translate.y, m_v3Translate.z);

		glTranslatef(m_v3Centre.x, m_v3Centre.y, m_v3Centre.z);

		glRotatef(m_v3RotateAngle.x, 1, 0, 0);
		glRotatef(m_v3RotateAngle.y, 0, 1, 0);
		glRotatef(m_v3RotateAngle.z, 0, 0, 1);

		glScalef(m_v3Scale.x, m_v3Scale.y, m_v3Scale.z);
		
		//glTranslatef(-m_v3Centre.x, -m_v3Centre.y, -m_v3Centre.z);
		//glTranslatef(-m_v3Translate.x, -m_v3Translate.y, -m_v3Translate.z);
		
		//move down the tree

		m_pIn->RenderOGL(bRenderSolid);

		//get old matrix
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		if(m_pNext)
			m_pNext->RenderOGL(bRenderSolid);
	}
}

