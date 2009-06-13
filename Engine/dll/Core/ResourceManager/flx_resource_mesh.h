/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _RESOURCEMESH2_H
#define _RESOURCEMESH2_H

#include <string>

#include "../../Assimp/include/assimp.h"
#include "../../Assimp/include/assimp.hpp"
#include "aiAssert.h"
#include "aiFileIO.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include "../../Core/flx_core.h"
#include "../../Core/flx_ogl_buffer.h"
#include "../../Core/flx_logger.h"
#include "../../Core/ResourceManager/flx_resource_manager.h"

//#pragma comment(lib, "../../../Assimp/lib/Assimp.lib")

using namespace Assimp;

struct flx_Face
{
	unsigned int start;
	unsigned int count;
};


class ResourceMesh2
{
public:
	ResourceMesh2();
	virtual ~ResourceMesh2();

	void Load(std::string _filename);
	void Draw(bool _selected = false, bool _renderTextures = true);

	aiScene* m_pScene;
	flx_Index* m_pIndices;

	std::vector<ResourceTexture2D*>			m_Textures;
	std::vector<flx_ogl_buffer<flx_Index>>  vIndexBuffers;
	flx_ogl_buffer<flx_Vertex>				*VertexBuffer;
	flx_ogl_buffer<flx_Normal>				*NormalBuffer;
	flx_ogl_buffer<flx_VertexColor>			*ColorBuffer;
	flx_ogl_buffer<flx_TexCoord>			*TexCoordBuffer;
	flx_ogl_buffer<flx_Index>				IndexBuffer;
};

#endif