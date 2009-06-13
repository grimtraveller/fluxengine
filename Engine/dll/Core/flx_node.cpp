/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_node.h"

Node::Node()
{
	m_pNext = NULL;
	m_pPrev = NULL;
	m_pIn	= NULL;
	m_pOut	= NULL;

	m_node_name.clear();
	m_bVisible	= true;
}

Node::~Node()
{
}

bool Node::SetName(const std::string &node_name)
{
	m_node_name = node_name;

	return true;
}

Node* Node::FindNode(const std::string &node_name)
{
	if(m_node_name.compare(node_name) == 0)
		return this;

	if (m_pIn)
	{
		Node* pNode;
		pNode = m_pIn->FindNode(node_name);
		if(pNode)
			return pNode;
	}

	//not found yet, look next
	if(m_pNext)
		return m_pNext->FindNode(node_name);

	return NULL;
}

Node* Node::FindNodeByID(unsigned int id)
{
	if(m_iID == id)
		return this;

	if (m_pIn)
	{
		Node* pNode;
		pNode = m_pIn->FindNodeByID(id);
		if(pNode)
			return pNode;
	}

	//not found yet, look next
	if(m_pNext)
		return m_pNext->FindNodeByID(id);

	return NULL;
}

unsigned Node::GetID()
{
	return m_iID;
}

void Node::SetID(unsigned int id)
{
	m_iID = id;
}

bool Node::isShape()
{
	if(m_Type == SHAPE)
		return true;

	return false;
}

bool Node::isTransform()
{
	if(m_Type == TRANSFORM)
		return true;

	return false;
}