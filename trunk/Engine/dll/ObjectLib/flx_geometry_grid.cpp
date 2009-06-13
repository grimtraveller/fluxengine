/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_geometry_grid.h"
#include "../Core/ResourceManager/flx_resource_manager.h"

GeometryFlatGrid::GeometryFlatGrid(float fWidth, float fDepth, int iDivX, int iDivZ)
{
	m_V = NULL;
	m_T = NULL;

	if(fWidth <= 0) fWidth = 1.0f;
	if(fDepth <= 0) fDepth = 1.0f;
	if(iDivX <= 0) iDivX = 1;
	if(iDivZ <= 0) iDivZ = 1;

	m_fDepth	= fDepth;
	m_fWidth	= fWidth;
	m_iDivX		= iDivX;
	m_iDivZ		= iDivZ;

	m_V = new Vertex3[(iDivX + 1) * (iDivZ + 1)];
	m_T = new Vertex2[(iDivX + 1) * (iDivZ + 1)];

	if(m_V && m_T) 
	{
		for (int z = 0; z < (iDivZ + 1); z++)
		{
			for (int x = 0; x < (iDivX + 1); x++)
			{
				m_V[x + z * (iDivX + 1)].m_x = -fWidth/2 + x*fWidth/iDivX;
				m_V[x + z * (iDivX + 1)].m_y = 0.0f;
				m_V[x + z * (iDivX + 1)].m_z = -fDepth/2 + z*fDepth/iDivZ;

				m_T[x+z*(iDivX+1)].x = (float)x;
				m_T[x+z*(iDivX+1)].y = (float)(iDivZ -z);
			}
		}
	}

	m_AABBox.vecMin = Vector3(0, 0, 0);
	m_AABBox.vecMax = Vector3(fWidth, 0, fDepth);
}

GeometryFlatGrid::~GeometryFlatGrid()
{
	if (m_V)
		SAFE_DELETE_ARRAY (m_V);
	if (m_T)
		SAFE_DELETE_ARRAY (m_T);
}

void GeometryFlatGrid::Update()
{
}

void GeometryFlatGrid::RenderOGL(bool bTexture)
{
	glNormal3f(0, 1, 0);

	if(bTexture)
	{
		for (int z = 0; z < m_iDivZ; z++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int x = 0; x < (m_iDivX + 1); x++)
			{
				glTexCoord2f(m_T[x+z	* (m_iDivX + 1)].x, m_T[x+z	*(m_iDivX +1)].y);
				glVertex3f(m_V[x + z	* (m_iDivX +1)].m_x, m_V[x + z	* (m_iDivX +1)].m_y, m_V[ x + z * (m_iDivX + 1)].m_z);

				glTexCoord2f( m_T[x + (z + 1)	* (m_iDivX +1)].x,	m_T[x + (z+1) * (m_iDivX +1) ].y);
				glVertex3f(m_V[x + (z+1)		* (m_iDivX +1)].m_x,	m_V[x + (z+1) * (m_iDivX +1)].m_y,	m_V[ x + (z+1) * (m_iDivX + 1)].m_z);
			}
			glEnd();
		}
	}
	else
	{
		for (int z = 0; z < m_iDivZ; z++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int x = 0; x < (m_iDivX + 1); x++)
			{
				glVertex3f(m_V[x + z		* (m_iDivX +1)].m_x,	m_V[x + z		* (m_iDivX +1)].m_y,		m_V[ x + z		* (m_iDivX + 1)].m_z);
				glVertex3f(m_V[x + (z+1)	* (m_iDivX +1)].m_x,	m_V[x + (z+1)	* (m_iDivX +1)].m_y,	m_V[ x + (z+1)	* (m_iDivX + 1)].m_z);
			}
			glEnd();
		}
	}
	return;
}
