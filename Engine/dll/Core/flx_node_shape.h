/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef NODESHAPE_H
#define NODESHAPE_H

#include "flx_node.h"
#include "flx_material.h"
#include "flx_texture_transform.h"
#include "../ObjectLib/flx_geometry.h"
#include "../Core/ResourceManager/flx_resource_shader.h"

class ENGINE_API NodeShape;

struct AlphaObject {
	NodeShape*	pShape;
	float		fDist2Eye;
	float		f16Matrix[16];
	
};

class NodeShape : public Node {
public:
	NodeShape(Geometry* pGeometry);
	virtual ~NodeShape();

	void Update();
	void RenderOGL(bool bSecondPass);
	bool ignoresFrustumCulling();
	void ignoresFrustumCulling(bool ignore);

public:
	Material			*m_pMaterial;
	GLint				m_pTexture[8];
	ResourceShader		*m_pShader;

	TextureTransform	* m_pTextureTransform;
	Geometry			*m_pGeometry;
	Matrix4x4			m_Matrix;
protected:

private:
	bool m_ignoreFrustumCulling;
	
};

#endif