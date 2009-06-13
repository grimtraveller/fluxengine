/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_graph.h"
#include "flx_renderer.h"

Graph::Graph(int _historyLimit, float _posx, float _posy, float _offsetx, float _offsety) 
:	m_maxHistory(_historyLimit),
	m_positionX(_posx),
	m_positionY(_posy),
	m_offsetX(_offsetx),
	m_offsetY(_offsety)
{
	for(int i = 0; i <= m_maxHistory; ++i)
		m_dataHistory.push_back(-1);

	m_colorR = 1.0f;
	m_colorG = 1.0f;
	m_colorB = 1.0f;
}

Graph::~Graph()
{
}

void Graph::update(float _value)
{
	//move the array data i to i + 1
	for(int i = 0; i < m_dataHistory.size()-1; ++i)	
		m_dataHistory[i] = m_dataHistory[i+1];

	m_dataHistory[m_maxHistory-1] = _value;
}

void Graph::render()
{
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(0,0,0);
	for(int i = 0; i < m_dataHistory.size()-1 ; ++i)
	{
		if(m_dataHistory[i] < 0) 
			continue;

		glColor3f(m_colorR, m_colorG, m_colorB);
		glBegin(GL_LINES);
		glVertex3f((i-1) * m_offsetX, m_dataHistory[(i-1 > 0) ? (i-1):i] * m_offsetY + m_positionY, 0);
		glVertex3f(i     * m_offsetX, m_dataHistory[i]					 * m_offsetY + m_positionY, 0);
		glEnd();
	}
	glEnable(GL_DEPTH_TEST);
}

void Graph::setColor(float _r, float _g, float _b)
{
	m_colorR = _r; m_colorG = _g; m_colorB = _b;
}