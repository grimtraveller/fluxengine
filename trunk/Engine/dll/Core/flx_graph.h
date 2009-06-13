/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>

class Graph {
public:
	Graph(int _historyLimit, float _posx = 0, float _posy = 0, float _offsetx = 1, float _offsety = 1);
	virtual ~Graph();

	void update(float _value);
	void render();
	void setColor(float _r, float _g, float _b);

private:
	std::vector<float>	m_dataHistory;
	unsigned int		m_maxHistory;

	float				m_positionX;
	float				m_positionY;
	float				m_offsetX;
	float				m_offsetY;

	float				m_colorR;
	float				m_colorG;
	float				m_colorB;
};

#endif