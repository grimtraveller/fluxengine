/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef NODETRANSFORM_H
#define NODETRANSFORM_H

#include "flx_node.h"
#include "../MathLib/flx_vector3.h"

class ENGINE_API NodeTransform : public Node
{
public:
	NodeTransform();
	virtual ~NodeTransform();

	void Update();
	void RenderOGL(bool bRenderSolid);

public:
	Vector3 m_v3Translate;
	Vector3 m_v3Centre;
	Vector3 m_v3Scale;
	Vector3 m_v3RotateAxis;
	Vector3 m_v3RotateAngle;

	Matrix4x4 m_pMatrix;
};

#endif