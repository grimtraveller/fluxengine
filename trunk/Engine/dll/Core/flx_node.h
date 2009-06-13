/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef NODE_H
#define NODE_H

#include "../Core/flx_core.h"
#include <string>
#include <vector>

enum NodeType {
	TRANSFORM = 0,
	SHAPE
};

struct AlphaObject;

class ENGINE_API Node {
public:
	Node();
	virtual ~Node();

	bool isTransform();
	bool isShape();

	bool			SetName(const std::string& node_name);
	Node*			FindNode(const std::string& node_name);
	Node*			FindNodeByID(unsigned int id);
	unsigned int	GetID();
	void			SetID(unsigned int id);


	virtual void Update() = 0;
	virtual void RenderOGL(bool bRenderSolid) = 0;
	
	bool m_bVisible;
public:
	static std::vector<AlphaObject*>* s_vAlphaObjects;

	Node* m_pPrev;
	Node* m_pNext;
	Node* m_pOut;
	Node* m_pIn;
protected:
	NodeType	m_Type;

public:
	std::string m_node_name;

	unsigned int m_iID;
};

#endif